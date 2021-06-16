using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(MeshFilter))]
public abstract class MeshProcessor : MonoBehaviour
{
    protected Mesh mesh;
    protected Vector3[] vertices;
    protected int[] triangles;

    void Awake()
    {
        mesh = GetComponent<MeshFilter>().mesh;
    }

    // Start is called before the first frame update
    protected void Start()
    {
        MakeMeshData();
        CreateMesh();
    }

    virtual public void MakeMeshData()
    {
    }

    // Update is called once per frame
    void Update()
    {
    }

    void CreateMesh()
    {
        mesh.Clear();
        mesh.vertices = vertices;
        mesh.triangles = triangles;

        mesh.RecalculateNormals();
    }
}
