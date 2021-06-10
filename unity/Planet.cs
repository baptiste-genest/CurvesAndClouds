using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Planet : MeshProcessor
{
    Vector3 acc, vel;
    float g = -10f;
    float dt = 0.01f;
    float mass;
    float radius;
    int rez = 30;

    void Start() 
    {
        float r = 5.0f;
        transform.position = (new Vector3(Random.Range(-r, r), Random.Range(-r, r), Random.Range(-r, r))).normalized*r;
        print(transform.position);
        acc = new Vector3();
        vel = new Vector3(1, 1, 0) * 1f;
        mass = Random.Range(5f, 20f);
        radius = mass / 10f;
        base.Start();
    }

    override public void MakeMeshData()
    {
        int rrez = rez - 1;
        vertices = new Vector3[rez * rez];
        for (int j = 0; j < rez; j++)
        {
            float theta = Mathf.PI * ((float)j / rrez);
            for (int i = 0; i < rez; i++)
            {
                float phi = 2 * Mathf.PI * ((float)i / rrez);
                vertices[j * rez + i] = new Vector3(
                    Mathf.Sin(theta) * Mathf.Cos(phi),
                    Mathf.Sin(theta) * Mathf.Sin(phi),
                    Mathf.Cos(theta)
                    )*radius;
            }
        }
        triangles = new int[rrez * rrez * 2 * 3];
        for (int j = 0; j < rrez; j++)
        {
            for (int i = 0; i < rrez; i++)
            {
                triangles[(j * rrez + i) * 6] = j * rez + i;
                triangles[(j * rrez + i) * 6 + 1] = (j + 1) * rez + i;
                triangles[(j * rrez + i) * 6 + 2] = j * rez + (i + 1);
                triangles[(j * rrez + i) * 6 + 4] = (j + 1) * rez + i;
                triangles[(j * rrez + i) * 6 + 3] = j * rez + (i + 1);
                triangles[(j * rrez + i) * 6 + 5] = (j + 1) * rez + (i + 1);
            }
        }
    }

    // Update is called once per frame
    void Update()
    {
        float dist2 = transform.position.sqrMagnitude;
        float force = g / dist2;
        acc = transform.position.normalized * force;
        vel += acc * dt;
        transform.position += vel * dt;
    }
}
