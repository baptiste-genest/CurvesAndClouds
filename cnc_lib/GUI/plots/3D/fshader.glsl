#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

uniform vec3 light_pos;
uniform vec3 cam_pos;
uniform int ambient_light;
uniform int color_mode;
uniform int specular;

varying vec3 v_normal;
varying vec3 v_position;
varying vec3 v_color;
varying float v_value;

void main()
{
        float light = 1.;
        vec3 I = normalize(light_pos - v_position);
        if (ambient_light == 1){
                light = max(0.,-dot(I,v_normal));
        }
        if (specular == 1 && dot(I,v_normal) < 0.f){
                vec3 R = reflect(I,v_normal);
                vec3 toCam = normalize(cam_pos - v_position);
                light += max(dot(R,toCam),0.f)*0.2f;
                clamp(light,0.f,1.f);
        }
        vec3 color = vec3(1.,1.,1.);
        if (color_mode == 1){
                float v = clamp(v_value,0.,1.);
                color = vec3(v,0.,1.-v);
        }
        else if (color_mode == 2)
                color = v_color;
        gl_FragColor = vec4(color,1.)*light;
}

