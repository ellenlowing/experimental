// Author: @patriciogv - 2015
// Title: Metaballs
#version 120
#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;
uniform float u_trigger1;
uniform float u_trigger2;
uniform float u_trigger3;
uniform float u_trigger4;
uniform float u_trigger5;
uniform float u_trigger6;
uniform float u_trigger7;

vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

float plot(vec2 st, float pct){
  return  smoothstep( pct-0.02, pct, st.y) -
          smoothstep( pct, pct+0.02, st.y);
}

float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

float cubicPulse( float c, float w, float x ){
    x = abs(x - c);
    if( x>w ) return 0.0;
    x /= w;
    return 1.0 - x*x*(3.0-2.0*x);
}

float pulse(vec2 st, float start, float end ) {
    return step(start, st.x) - step(end, st.x);
}

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    // st.x *= u_resolution.x/u_resolution.y;
    vec3 color = vec3(.0);
	  float scale = 6.;
    // Scale
    st *= scale;

    // Tile the space
    vec2 i_st = floor(st); // global index
    vec2 f_st = fract(st); // local index
    float m_dist = 3. * scale;  // minimun distance

    // First parameter determines which section to be triggered
    // Only from 1 to 7
    float sin_val = 0.;
    // float sin_val = (-scale*1.) * cubicPulse(0., 1., st.x);
    sin_val += scale * u_trigger1 * cubicPulse(0., 1., st.x);
    sin_val += scale * u_trigger2 * cubicPulse(1., 1., st.x);
    sin_val += scale * u_trigger3 * cubicPulse(2., 1., st.x);
    sin_val += scale * u_trigger4 * cubicPulse(3., 1., st.x);
    sin_val += scale * u_trigger5 * cubicPulse(4., 1., st.x);
    sin_val += scale * u_trigger6 * cubicPulse(5., 1., st.x);
    sin_val += scale * u_trigger7 * cubicPulse(6., 1., st.x);

    float speed = 0.;
    float speedscale = 4.;
    // speed += speedscale * u_trigger1 * pulse(st, 0., 1.) + 0.1;
    // speed += speedscale * u_trigger2 * pulse(st, 0., 2.) + 0.1;
    // speed += speedscale * u_trigger3 * pulse(st, 1., 3.) + 0.1;
    // speed += speedscale * u_trigger4 * pulse(st, 2., 4.) + 0.1;
    // speed += speedscale * u_trigger5 * pulse(st, 3., 5.) + 0.1;
    // speed += speedscale * u_trigger6 * pulse(st, 4., 6.) + 0.1;
    // speed += speedscale * u_trigger7 * pulse(st, 5., 6.) + 0.1;
    // speed = clamp(speed, 0.1, speedscale);
    // float pct = plot(st, sin_val);
    // float speedpct = plot(st, speed);

    for (int j= -1; j <= 1; j++ ) {
        for (int i= -1; i <= 1; i++ ) {
            // Neighbor place in the grid
            vec2 neighbor = vec2(float(i),float(j));

            // Random position from current + neighbor place in the grid
            vec2 offset = random2(i_st + neighbor);

            // Animate the offset
            // Add position dependent
            offset = 0.5 + 0.5*sin(u_time*2. + 6.14*offset);

            // Position of the cell
            vec2 pos = neighbor + offset - f_st;

            // Cell distance
            float dist = length(pos);

            // Metaball it!
            m_dist = min(m_dist, m_dist*dist);
        }
    }

    // Draw cells
    // The higher the first parameter, the easier metaball is drawn
    float val = map(sin_val, 0., scale-1, 0.3, 0.3 * scale);
    color += step(val, m_dist);


    gl_FragColor = vec4(1.-color, 1.0);
}
