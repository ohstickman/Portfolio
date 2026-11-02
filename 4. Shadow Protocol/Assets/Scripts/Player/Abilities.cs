using System.Collections;
using UnityEngine;
using UnityEngine.UI;

public class Abilities: MonoBehaviour
{
    public Material mat;
    public Material invisible;

    Outline outline;

    public Renderer mesh;

    public static Abilities Instance;

    public ParticleSystem Exp;

    public bool isInvisible;
    public float waitBeforeInvisible = 0.25f;

    public float SecsUse = 2f;
    public float SecsReload = 60f;
    public Image card;

    bool canUse = true;

    public float speed = 5f;

    public Image bar;
    public GameObject FullBar;

    void Start()
    {
        mat = mesh.material;
        outline = mesh.GetComponent<Outline>();
        card.fillAmount = 1f;
        FullBar.SetActive(false);
        Instance = this;
    }

    public void Invisible()
    {
        if (!canUse) return;
        if(PlayerController.Instance.isLock) return;

        canUse = false;
        isInvisible = true;
        
        Exp.Play();
        card.fillAmount = 0f;
        bar.fillAmount = 1f;
        FullBar.SetActive(true);
        
        StartCoroutine(inv());
        StartCoroutine(Use());
    }

    IEnumerator Use()
    {
        float elapsedTime = 0f;

        while (elapsedTime < SecsUse)
        {
            elapsedTime += Time.deltaTime;
            bar.fillAmount = 1 - Mathf.Clamp01(elapsedTime / SecsUse);
            yield return null;
        }

        bar.fillAmount = 0f;
        FullBar.SetActive(false);
        
        isInvisible = false;
        Exp.Play();
        StartCoroutine(inv());
        
        StartCoroutine(Reload());
    }

    IEnumerator Reload()
    {
        float elapsedTime = 0f;

        while (elapsedTime < SecsReload)
        {
            elapsedTime += Time.deltaTime;
            card.fillAmount = Mathf.Clamp01(elapsedTime / SecsReload);
            yield return null;
        }
        
        card.fillAmount = 1f;
        canUse = true;
    }

    IEnumerator inv()
    {
        yield return new WaitForSeconds(waitBeforeInvisible);
        if(isInvisible) {
            mesh.material = invisible; 
            outline.OutlineMode = Outline.Mode.OutlineAndSilhouette;
        }
        else{
            mesh.material = mat;
            outline.OutlineMode = Outline.Mode.SilhouetteOnly;
        }
    }

    void Update()
    {
        if(PlayerController.Instance.isLock) return;
        if (Input.GetKeyDown(KeyCode.E))
        {
            Invisible();
        }
    }
}