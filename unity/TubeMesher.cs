using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TubeMesher : MeshProcessor
{
    private List<Vector3> simplex;
    private float radius;
    private int resolution;
    public TubeMesher(){
    }

    public void build(List<Vector3> in_vertices,int _resolution,float rad){

        simplex = in_vertices;
        radius = rad;
        resolution = _resolution;
        Start();
    }

    // Start is called before the first frame update
    void Start()
    {
        base.Start();
    }

    override public void MakeMeshData()
    {
        int N = simplex.Count;
        vertices = new Vector3[resolution*N];
        uint vert_count = 0;
        Vector3 reference = new Vector3(0.321321f,0.5341f,0.5514f);
        for (int k = 0;k<N;k++){
            Vector3 p = simplex[k];
            Vector3 axis = simplex[(k+1)%N] - simplex[k];
            Vector3 perp = Vector3.Cross(axis,reference).normalized*radius;
            for (int i = 0;i<resolution;i++){
                float angle = i*360f/(resolution-1);
                Quaternion Rotate = Quaternion.AngleAxis(angle,axis);
                vertices[vert_count] = p + Rotate*perp;
                vert_count++;
            }
        }
        int reduced_resolution = resolution-1;
        int reduced_N = N-1;
        triangles = new int[reduced_resolution*reduced_N*6];
        for (int k = 0;k<reduced_N;k++){
            for (int i = 0;i<reduced_resolution;i++){
                triangles[(k * reduced_resolution + i) * 6] =     (k * resolution + i)%(resolution*reduced_N);
                triangles[(k * reduced_resolution + i) * 6 + 1] = (k * resolution + i + 1)%(resolution*reduced_N);
                triangles[(k * reduced_resolution + i) * 6 + 2] = ((k + 1) * resolution + i)%(resolution*reduced_N);
                triangles[(k * reduced_resolution + i) * 6 + 3] = ((k) * resolution + i + 1)%(resolution*reduced_N);
                triangles[(k * reduced_resolution + i) * 6 + 4] = ((k + 1) * resolution + i + 1)%(resolution*reduced_N);
                triangles[(k * reduced_resolution + i) * 6 + 5] = ((k + 1) * resolution + i)%(resolution*reduced_N);
            }
        }

    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
