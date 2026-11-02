using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(MeshFilter))]
[RequireComponent(typeof(MeshRenderer))]
public class SimpleFOV : MonoBehaviour
{
    [Header("Настройки Обзора")]
    public float viewRadius = 10f;
    [Range(0, 360)]
    public float viewAngle = 90f;
    
    [Header("Настройки Физики (Слои)")]
    public LayerMask playerLayer;
    public LayerMask enemyLayer;
    public LayerMask obstacleMask;

    [Header("Качество конуса на полу")]
    [Range(1, 10)]
    public int rayCountMultiplier = 1; 

    [Header("Настройки Цвета")]
    public Color defaultColor = Color.white;
    public Color alertColor = Color.red;
    public float colorChangeSpeed = 5f;

    [HideInInspector]
    public Transform visiblePlayer;
    public Transform diedEnemy;
    public bool isInv = false;

    private MeshFilter meshFilter;
    private Mesh fovMesh;
    private MeshRenderer meshRenderer;
    private Material fovMaterial;
    public bool isDie = false;

    private List<Vector3> viewPoints;
    private Vector3[] vertices;
    private int[] triangles;
    private int maxRays;

    private static float globalTimer = 0f;
    public float updateRate = 0.1f; 

    List<GameObject> nos;

    void Start()
    {
        meshFilter = GetComponent<MeshFilter>();
        meshRenderer = GetComponent<MeshRenderer>();
        
        fovMesh = new Mesh();
        fovMesh.name = "FOV Mesh";
        meshFilter.mesh = fovMesh;

        fovMaterial = new Material(meshRenderer.sharedMaterial);
        meshRenderer.material = fovMaterial;
        fovMaterial.color = defaultColor;

        maxRays = Mathf.RoundToInt(360 * rayCountMultiplier);
        viewPoints = new List<Vector3>(maxRays);
        vertices = new Vector3[maxRays + 1];
        triangles = new int[(maxRays - 1) * 3];

        nos = new List<GameObject>();
    }

    void Update()
    {
        globalTimer += Time.deltaTime;
    }

    void LateUpdate()
    {
        if (isDie)
        {
            if(meshFilter.mesh != null) meshFilter.mesh = null;
            return;
        }

        UpdateColor();

        if (globalTimer >= updateRate)
        {
            FindPlayerWithRaycast();
            DrawFOV();
            if (GetInstanceID() == GetLowestInstanceID()) 
            {
                globalTimer = 0f;
            }
        }
    }
    
    private int GetLowestInstanceID()
    {
        int lowest = GetInstanceID();
        SimpleFOV[] all = FindObjectsByType<SimpleFOV>(FindObjectsSortMode.None);
        foreach(var f in all) if(f.GetInstanceID() < lowest) lowest = f.GetInstanceID();
        return lowest;
    }

    void FindPlayerWithRaycast()
    {
        isInv = false;
        visiblePlayer = null;
        diedEnemy = null;

        Collider[] targetsInRadius = Physics.OverlapSphere(transform.position, viewRadius, playerLayer);
        Collider[] enemsInRadius = Physics.OverlapSphere(transform.position, viewRadius, enemyLayer);

        if (targetsInRadius.Length > 0)
        {
            Transform target = targetsInRadius[0].transform;
            Vector3 dirToTarget = (target.position - transform.position).normalized;

            if (Vector3.Angle(transform.forward, dirToTarget) < viewAngle / 2)
            {
                float dstToTarget = Vector3.Distance(transform.position, target.position);

                if (!Physics.Raycast(transform.position + Vector3.up * 0.5f, dirToTarget, dstToTarget, obstacleMask))
                {
                    isInv = target.GetComponent<Abilities>().isInvisible;
                    isDie = PlayerController.Instance.Health == 0 ? true : false;
                    if(!isInv && !isDie) visiblePlayer = target;
                }
            }
        }

        if (enemsInRadius.Length > 0)
        {
            foreach (Collider col in enemsInRadius)
            {
                Enemy otherEnemy = col.GetComponentInParent<Enemy>();
                if (otherEnemy == this) continue;
                
                if (otherEnemy != null && !otherEnemy.isAlive) 
                {
                    Transform target = otherEnemy.transform;
                    if (nos.Contains(target.gameObject)) continue;
                    Vector3 dirToTarget = (target.position - transform.position).normalized;

                    if (Vector3.Angle(transform.forward, dirToTarget) < viewAngle / 2)
                    {
                        float dstToTarget = Vector3.Distance(transform.position, target.position);
                        if (!Physics.Raycast(transform.position + Vector3.up * 0.5f, dirToTarget, dstToTarget, obstacleMask))
                        {
                            diedEnemy = col.transform;
                            nos.Add(target.gameObject);
                            break;
                        }
                    }
                }
            }
        }
    }

    void UpdateColor()
    {
        Color targetColor = (visiblePlayer != null) ? alertColor : defaultColor;
        fovMaterial.color = Color.Lerp(fovMaterial.color, targetColor, colorChangeSpeed * Time.deltaTime);
    }

    void DrawFOV()
    {
        int numRays = Mathf.RoundToInt(viewAngle * rayCountMultiplier);
        float angleStep = viewAngle / numRays;
        viewPoints.Clear();

        for (int i = 0; i <= numRays; i++)
        {
            float currentAngle = transform.eulerAngles.y - viewAngle / 2 + angleStep * i;
            Vector3 dir = DirFromAngle(currentAngle);

            RaycastHit hit;
            if (Physics.Raycast(transform.position + Vector3.up * 0.1f, dir, out hit, viewRadius, obstacleMask))
            {
                viewPoints.Add(hit.point);
            }
            else
            {
                viewPoints.Add(transform.position + dir * viewRadius);
            }
        }

        int vertexCount = viewPoints.Count + 1;
        vertices[0] = Vector3.zero;
        for (int i = 0; i < vertexCount - 1; i++)
        {
            vertices[i + 1] = transform.InverseTransformPoint(viewPoints[i]);
            if (i < vertexCount - 2)
            {
                triangles[i * 3] = 0;
                triangles[i * 3 + 1] = i + 1;
                triangles[i * 3 + 2] = i + 2;
            }
        }

        fovMesh.Clear();
        fovMesh.SetVertices(vertices, 0, vertexCount); 
        fovMesh.SetTriangles(triangles, 0, (vertexCount - 2) * 3, 0); 
        fovMesh.RecalculateNormals();
    }

    private Vector3 DirFromAngle(float angleInDegrees)
    {
        return new Vector3(Mathf.Sin(angleInDegrees * Mathf.Deg2Rad), 0, Mathf.Cos(angleInDegrees * Mathf.Deg2Rad));
    }
}