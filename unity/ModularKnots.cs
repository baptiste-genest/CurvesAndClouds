using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ModularKnots : MonoBehaviour
{
    private Shader S;
    private Texture T;
    // Start is called before the first frame update
    void Start()
    {
        GameObject trefoil =
         new GameObject("trefoil knot");
        trefoil.AddComponent<MeshFilter>();
        trefoil.AddComponent<MeshRenderer>();
        List<Vector3> V = new List<Vector3>();
        int N = 60;
        int n = N-1;
        for (uint k = 0;k<N;k++){
            float t = 2*Mathf.PI*k/n;
            V.Add(new Vector3(
                Mathf.Cos(t) + 2*Mathf.Cos(2*t),
                Mathf.Sin(t) - 2*Mathf.Sin(2*t),
                2*Mathf.Sin(3*t)));
        }
        TubeMesher TM = trefoil.AddComponent<TubeMesher>();
        TM.build(V,30,.1f);

        Renderer rend = trefoil.GetComponent<Renderer>();
        rend.material = new Material(Shader.Find("Specular"));
        rend.material.mainTexture = T;
        rend.material.color = Color.red;
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
