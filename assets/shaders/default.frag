#version 330 core
out vec4 FragColor;

#define PI 3.1415926535897932384626433832795
#define DOT_CLAMP 0.00001


in VS_OUT {
    vec3 normal;
    vec2 texcoords_2d;
    vec3 light_vec;
    vec3 view_vec;
} fs_in;



// light uniforms
uniform vec3 point_light; // Ii
uniform vec3 ambient_light; // Ia
uniform float light_intensity;

// surface uniforms
uniform sampler2D diffuse_texture;
uniform vec3 specular; // Ks
uniform float shininess; // alpha exponent




void main()
{
    vec3 Kd = texture(diffuse_texture, fs_in.texcoords_2d).rgb;

    vec3 Ks = specular;
    float alpha = shininess;

    vec3 V = normalize(fs_in.view_vec);
    vec3 L = normalize(fs_in.light_vec);
    vec3 N = normalize(fs_in.normal);
    vec3 H = normalize(L+V);

    float distance_falloff = 1 / length(fs_in.light_vec) * light_intensity;

    vec3 IaKd = ambient_light * Kd;    
    vec3 IiKd = point_light * Kd;
    vec3 IiKs = point_light * Ks;

    float NdotL = max(DOT_CLAMP, dot(N, L));
    float NdotH = max(DOT_CLAMP, dot(N, H));
    
    float VdotN = max(DOT_CLAMP, dot(V, N));    
    float LdotH = max(DOT_CLAMP, dot(L, H));

    float NdotH_alpha = pow(NdotH, alpha);

   // A diffuse light part of BRDF. 
    vec3 KdOverPi = Kd / PI;

    // The Schlick approximation to the Fresnel term F    
    vec3 F = Ks + (vec3(1)-Ks) * pow(1-LdotH, 5);
    
    // an approximation to G(L, V, H) / (dot(L,N)*dot(V,N)). G(L, V, H) Represents how
    // much is self-occluded. That is, what percent of microfacet normals reflect light
    // toward another microfacet. I think... might be wrong. 
    float Gcheat = 1 / pow(LdotH, 2);
    
    // The micro-facet normal distribution term D (what part of microfacets
    // reflects in V direction). There's a similarity to blinn phong.
    float D = ( (alpha + 2)/(2 * PI) ) * NdotH_alpha;

    // KdPi + F(L, H) * G(L, V, H) * D(H) / (4 * ldotn * VdotN)
    vec3 brdf = KdOverPi + F * Gcheat * D / 4;

    // Blinn Phong
    //vec3 I = IaKd + IaKd * NdotL + IiKs * NdotH_alpha;
    
    // BRDF
    vec3 I = IaKd + point_light * NdotL  * brdf * distance_falloff;

    //I = I + vec3(1, 0, 0);

    FragColor = vec4(I, 1);
}