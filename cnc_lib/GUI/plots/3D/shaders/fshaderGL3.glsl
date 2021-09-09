#version 300 es
in highp vec3 vert;
in highp vec3 vertNormal;
in highp vec3 color;

out highp vec4 fragColor;

uniform highp vec3 lightpos;

void main()
{
    highp vec3 l = normalize(lightpos - vert);
    highp float nl = max(dot(normalize(vertNormal), l), 0.0);
    highp vec3 col = clamp(color * 0.2 + color * 0.8 * nl, 0.0, 1.0);
    fragColor = vec4(col, 1.0);
}

