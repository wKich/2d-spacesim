uniform vec2 qt_Resolution;
uniform vec2 qt_BackgroundOffset;

void main(void) {
    vec2 p = (gl_FragCoord.xy / qt_Resolution.xy);
    float width = qt_Resolution.x;
    float height = qt_Resolution.y;
    float x = sin(p.x*3.14*width/32.0 + qt_BackgroundOffset.x);
    float y = sin(p.y*3.14*height/33.75 + qt_BackgroundOffset.y);
    vec3 color = vec3(sign(x*y)*0.1);

    gl_FragColor = vec4(color, 1.0);
}
