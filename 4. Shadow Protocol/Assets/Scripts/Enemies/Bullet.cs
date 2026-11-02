using UnityEngine;

public class Bullet : MonoBehaviour
{
    public Vector3 dir;
    public float speed = 50f;
    public float lineWidth = 0.5f;
    public float lineLong = 5f;
    public LayerMask pl;
    public int Damage = 10;
    
    private LineRenderer lr;
    private Vector3 startPos;

    void Start()
    {
        startPos = transform.position;
        lr = GetComponent<LineRenderer>();
        
        lr.startWidth = lineWidth;
        lr.endWidth = lineWidth;

        transform.forward = dir.normalized;
        
        Destroy(gameObject, 10f);
    }

    void Update()
    {
        transform.Translate(dir.normalized * speed * Time.deltaTime, Space.World);

        lr.SetPosition(0, startPos);
        lr.SetPosition(1, transform.position);

        if (Vector3.Distance(startPos, transform.position) > lineLong)
        {
            startPos = transform.position - dir.normalized * lineLong;
        }
    }

    void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag("Player"))
            other.GetComponent<PlayerController>().TakeDamage(Damage, transform);
        
        Destroy(gameObject);
    }
}