using System.Collections;
using TMPro;
using UnityEngine;

public class PlayerController : MonoBehaviour
{
    public Camera cam;
    public GameObject player;
    public Animator pAnimator;
    public float speed = 8;
    public float camSpeed;
    public Vector3 camOffset;
    public Vector3 camOffsetDie;

    CharacterController controller;
    public ParticleSystem ps;

    public int Health = 100;
    public int MaxHealth = 100;

    public static PlayerController Instance;

    public Color LowColor = Color.red;
    public Color HighColor = Color.cyan;
    public Renderer mesh;
    public Material mat;

    public TextMeshProUGUI info;
    public Transform parent;

    public float speedMsg = 70f; 
    public float fadeMsg = 10f; 

    public Vector2 BeginPos;
    public Vector2 EndPos;
    public bool isLock = true;


    public VariableJoystick vj;

public void TakeDamage(int damage, Transform bul)
{
    ps.Play();
    Health = Mathf.Max(0, Health - damage);
    CameraShake.Instance.Shake(0.1f, 0.1f);
    StartCoroutine(damColor());

    if (Health == 0)
    {
        Rigidbody[] rbs = GetComponentsInChildren<Rigidbody>();
        controller.enabled = false;
        foreach (Rigidbody c in rbs)
        {
            c.isKinematic = false;
        }

        Vector3 dir = bul.position;
        StartCoroutine(Die(dir));
    }
}

IEnumerator Die(Vector3 pos)
{
    isLock = true;
    pAnimator.enabled = false;

    yield return null;

    Grid.Instance.Ex(0);

    Rigidbody mainRb = GetComponentInChildren<Rigidbody>();

    Vector3 dir = transform.position - pos;
        
    dir.y = 0; 
    dir = dir.normalized;

    mainRb.AddForce(dir * 150f, ForceMode.Impulse);

    Time.timeScale = 0.3f;
    yield return new WaitForSecondsRealtime(3);
    
    Time.timeScale = 1f;
    yield return new WaitForSecondsRealtime(2);
    
    SceneReloader.Instance.Load(UnityEngine.SceneManagement.SceneManager.GetActiveScene().name);
}


    bool canNextMsg = true;

    public void msg(string txt)
    {
        if(canNextMsg) StartCoroutine(showtxt(txt));
    }

    IEnumerator showtxt(string msg)
    {
        canNextMsg = false;
        TextMeshProUGUI txt = Instantiate(info, parent);
        txt.text = msg;
        txt.rectTransform.anchoredPosition = BeginPos;

        Color startColor = txt.color;
        startColor.a = 0;
        txt.color = startColor;

        while (Vector2.Distance(txt.rectTransform.anchoredPosition, EndPos) > 0.05f || txt.color.a < 0.95f)
        {
            txt.rectTransform.anchoredPosition = Vector2.Lerp(txt.rectTransform.anchoredPosition, EndPos, speedMsg * Time.deltaTime);
            
            Color alpha = txt.color;
            alpha.a = Mathf.Lerp(alpha.a, 1f, fadeMsg * Time.deltaTime);
            txt.color = alpha;

            yield return null;
        }

        txt.rectTransform.anchoredPosition = EndPos;
        Color fullAlpha = txt.color;
        fullAlpha.a = 1f;
        txt.color = fullAlpha;

        yield return new WaitForSeconds(0.5f);

        while (txt.color.a > 0.05f)
        {
            Color alpha = txt.color;
            alpha.a = Mathf.Lerp(alpha.a, 0f, fadeMsg * Time.deltaTime);
            txt.color = alpha;

            yield return null;
        }

        Destroy(txt.gameObject);
        yield return new WaitForSeconds(2f);
        canNextMsg = true;
    }

    IEnumerator damColor()
    {
        mat.color = Color.white;

        yield return new WaitForSeconds(0.05f);

        float precent = Mathf.Clamp01((float)Health / MaxHealth);
        mat.color = Color.Lerp(LowColor, HighColor, precent);
    }

    

    void Start()
    {
        if(cam == null)
        {
            cam = Camera.main;
        }
        

        if(player == null)
        {
            player = gameObject;
        }

        if(pAnimator == null)
        {
            pAnimator = player.GetComponent<Animator>();
        }

        Instance = this;
        foreach(Rigidbody c in GetComponentsInChildren<Rigidbody>())
        {
            c.isKinematic = true;
        }

        

        cam.transform.position = player.transform.position + camOffset;
        cam.transform.position = new Vector3(cam.transform.position.x, cam.transform.position.y, 0);

        controller = player.GetComponent<CharacterController>();
        mat = mesh.material;
        mat.color = HighColor;
    }

    void Update()
    {

        if (Health != 0)
        {
            Vector3 RideTo = new Vector3(
                player.transform.position.x + camOffset.x, 
                player.transform.position.y + camOffset.y, 
                player.transform.position.z + camOffset.z
            );

            cam.transform.position = Vector3.Lerp(cam.transform.position, RideTo, camSpeed * Time.deltaTime);
        }
        else
        {
            Vector3 RideTo = new Vector3(
                player.transform.position.x + camOffsetDie.x, 
                player.transform.position.y + camOffsetDie.y, 
                player.transform.position.z + camOffsetDie.z
            );

            cam.transform.position = Vector3.Lerp(cam.transform.position, RideTo, camSpeed * Time.deltaTime);
        }

        if(isLock) return;
        float moveX = vj.Vertical;
        float moveZ = vj.Horizontal;

        Vector3 move = new Vector3(-moveX, 0, moveZ);
        controller.Move(move * speed * Time.deltaTime);

        if (move != Vector3.zero) 
        {
            Quaternion targetRotation = Quaternion.LookRotation(move);
            player.transform.rotation = Quaternion.Slerp(player.transform.rotation, targetRotation, 10f * Time.deltaTime);
        }

        if (move != Vector3.zero)
        {
            pAnimator.SetBool("isRun", true);
        }
        else
        {
            pAnimator.SetBool("isRun", false);
        }

    }
}
