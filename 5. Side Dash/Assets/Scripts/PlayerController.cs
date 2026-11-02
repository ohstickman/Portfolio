using UnityEngine;
using UnityEngine.UI;

public class PlayerController : MonoBehaviour
{
    public GameObject StayPose;
    public GameObject DashPose;

    public float dashDistance = 3f;
    public float dashDuration = 0.15f;

    private Vector3 targetPosition;
    private Vector3 startPosition;
    private float dashTimer = 0f;
    private bool isDashing = false;

    CharacterController cc;

    public ParticleSystem ps;

    public GameObject but;


    void Start()
    {
        DashPose.SetActive(false);
        cc = GetComponent<CharacterController>();
        but.SetActive(false);
    }

    void Update()
    {
        float moveX = 0;
        float moveZ = 0;

        if (Input.GetKeyDown(KeyCode.D)) moveX = 1;
        if (Input.GetKeyDown(KeyCode.A)) moveX = -1;
        if (Input.GetKeyDown(KeyCode.W)) moveZ = 1;
        if (Input.GetKeyDown(KeyCode.S)) moveZ = -1;

        if (moveX != 0 || moveZ != 0)
        {
            StartDash(new Vector3(moveX, 0, moveZ));
        }

        if (isDashing)
        {
            dashTimer += Time.deltaTime;
            float t = dashTimer / dashDuration;
            
            Vector3 target = Vector3.Lerp(startPosition, targetPosition, t);
            Vector3 d = target - transform.position;
            cc.Move(d);

            if (t >= 1f)
            {
                ps.Stop();
                isDashing = false;
                DashPose.SetActive(false);
                StayPose.SetActive(true);
            }
        }
    }

    void StartDash(Vector3 dir)
    {
        ps.Play();
        startPosition = transform.position; 
        targetPosition = startPosition + (dir * dashDistance);

        transform.rotation = Quaternion.LookRotation(dir);

        dashTimer = 0f;
        isDashing = true;

        DashPose.SetActive(true);
        StayPose.SetActive(false);
    }

    void OnTriggerEnter(Collider other) 
    {
        if (other.CompareTag("Wall")){
            but.SetActive(true);
            Destroy(gameObject);
        }
    }
}