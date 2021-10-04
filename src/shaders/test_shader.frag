
uniform vec2 windowSize;
uniform float elapsedTime;

uniform vec2 mousePos;

void main()
{
    vec2 col = mousePos / windowSize;
    gl_FragColor = vec4(col.x, col.y, abs(sin(elapsedTime)), 1.0f);
}
