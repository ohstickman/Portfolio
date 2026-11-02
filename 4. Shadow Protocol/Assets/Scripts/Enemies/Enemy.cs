using UnityEngine;
using UnityEngine.AI;
using System.Collections.Generic;
using System.Collections;
using UnityEngine.UI;
using UnityEngine.Animations;

public class Enemy : MonoBehaviour
{

    public enum type{Active, nonActive};
    public type enemytype;
    public float rotationSpeed = 10f;

    public Vector3[] angles;

    public bool isAlive = true;

    public Animator eAnimator;

    Transform player;
    public float InvDest = 1;
    public float ToInvDest = 5;

    public List<Transform> points;
    public float Force = 10;

    public NavMeshAgent enem;

    public Transform spawnBullet;

    public GameObject bullet;
    public float bulletSpeed;
    public float lineWidth = 5;
    public float lineLong = 5;
    public float WaitBull = 1;
    public int Damage = 5;

    public float duration = 0.2f;
    public float magnitude = 10;


    bool canShoot = true;

    SimpleFOV fovEnem;

    int NowPoint = 0;

    public ParticleSystem Blood;

    public Image what;
    public Image whay;

    public float EmotionSpeed = 10f;

    Vector3 targetWhay = Vector3.zero;
    Vector3 targetWhat = Vector3.zero;

    void Start()
    {
        foreach(Rigidbody c in gameObject.GetComponentsInChildren<Rigidbody>())
        {
            c.isKinematic = true;
        }

        ConstraintSource src = new ConstraintSource();
        src.sourceTransform = Camera.main.transform;
        src.weight = 1f;

        whay.GetComponentInParent<LookAtConstraint>().AddSource(src);
        
        player = GameObject.FindWithTag("Player").transform;
        eAnimator = GetComponent<Animator>();

        fovEnem = GetComponent<SimpleFOV>();
        enem = GetComponent<NavMeshAgent>();

        GoToNextPoint();



    }

    public void Kill(Vector3 pos)
    {
        if(!isAlive) return;

        Outline[] obj = GetComponentsInChildren<Outline>();
        foreach(Outline c in obj){
            c.enabled = false;
        }

        Grid.Instance.Ex(1);

        CameraShake.Instance.Shake(duration, magnitude);

        GetComponent<MoneyDrop>().Drop();

        enem.enabled = false;

        isAlive = false;
        eAnimator.enabled = false;
        
        foreach(Rigidbody c in gameObject.GetComponentsInChildren<Rigidbody>())
        {
            c.isKinematic = false;
        } 

        fovEnem.isDie = !isAlive;

        targetWhat = Vector3.zero;
        targetWhay = Vector3.zero;
        
        GetComponentInChildren<Rigidbody>().AddForce((transform.position - pos).normalized * Force, ForceMode.Impulse);
        GetComponent<Collider>().enabled = false;
        Blood.Play();


        //StartCoroutine(UnvisibleDestroy());
    }

    void GoToNextPoint()
    {
        if (points.Count == 0) return;
        enem.destination = points[NowPoint].position;
        NowPoint = (NowPoint + 1) % points.Count;
    }

    Vector3 lastPlayerPos = Vector3.zero;
    Vector3 diedPos = Vector3.zero;

    float size = 7;

    Vector3 targetRotation;

    void Update()
    {
        whay.rectTransform.localScale = Vector3.Lerp(whay.rectTransform.localScale, targetWhay, EmotionSpeed * Time.deltaTime);
        what.rectTransform.localScale = Vector3.Lerp(what.rectTransform.localScale, targetWhat, EmotionSpeed * Time.deltaTime);

        if (!isAlive) return;

        bool enable = whay.rectTransform.localScale.x > 0.2 ? true : false;
        whay.gameObject.SetActive(enable);
        
        enable = what.rectTransform.localScale.x > 0.2 ? true : false;
        what.gameObject.SetActive(enable);

        if(fovEnem.visiblePlayer != null && !fovEnem.isInv)
        {
            eAnimator.SetBool("isFire", true);
            Vector3 lookPos = player.position;
            lookPos.y = transform.position.y;
            lastPlayerPos = lookPos;
            transform.LookAt(lookPos);
            enem.destination = transform.position;
            if(canShoot) StartCoroutine(fireWait());
            targetWhat = Vector3.zero;
            targetWhay = Vector3.zero;
            
            diedPos = Vector3.zero;
        }
        else if (diedPos != Vector3.zero && enemytype == type.Active)
        {
            targetWhay = new Vector3(size, size, size);
            enem.destination = diedPos;

            if (!enem.pathPending && enem.remainingDistance < 1f)
            {
                targetWhay = Vector3.zero;
                diedPos = Vector3.zero;
                GoToNextPoint();
            }
        }
        else if (fovEnem.diedEnemy != null && enemytype == type.Active)
        {
            diedPos = fovEnem.diedEnemy.position;
        }
        else
        {
            eAnimator.SetBool("isFire", false);
            bool isDie = PlayerController.Instance.Health == 0 ? true : false;

            if(lastPlayerPos != Vector3.zero && enemytype == type.Active && !isDie)
            {
                enem.destination = lastPlayerPos;
                lastPlayerPos = Vector3.zero;
                targetWhat = new Vector3(size, size, size);
                return;
            }

            else if (!enem.pathPending && enem.remainingDistance < 0.5f && enemytype == type.Active)
            {
                targetWhat = Vector3.zero;
                targetWhay = Vector3.zero;
                GoToNextPoint();
            }
            else if(enemytype == type.nonActive)
            {
                transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.Euler(targetRotation), rotationSpeed * Time.deltaTime);

                if (Quaternion.Angle(transform.rotation, Quaternion.Euler(targetRotation)) < 0.1f)
                {
                    targetRotation = angles[Random.Range(0, angles.Length)];
                }
            }
        }
    }

    IEnumerator fireWait()
    {
        canShoot = false;
        yield return new WaitForSeconds(WaitBull);

        if(isAlive && fovEnem.visiblePlayer != null && !fovEnem.isInv) Fire();
        canShoot = true;
    }

    void Fire()
    {
        GameObject bulletobj = Instantiate(bullet, spawnBullet.position, spawnBullet.rotation);

        Bullet bulletSc = bulletobj.GetComponent<Bullet>();

        //pFire.Play();

        bulletSc.speed = bulletSpeed;
        bulletSc.dir = transform.forward;
        bulletSc.lineWidth = lineWidth;
        bulletSc.lineLong = lineLong;
        bulletSc.Damage = Damage;
    }
}
