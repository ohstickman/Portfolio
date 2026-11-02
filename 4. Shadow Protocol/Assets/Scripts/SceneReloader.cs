using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using System.Collections;

public class SceneReloader : MonoBehaviour
{
    public static SceneReloader Instance;

    public Image left;
    public Image right;

    public float speed = 5f;

    public Vector3 leftOpenPos;
    public Vector3 leftClosedPos;
    public Vector3 rightOpenPos;
    public Vector3 rightClosedPos;

    void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
        }
        else
        {
            Destroy(gameObject);
        }
    }

    void Start()
    {
        left.transform.localPosition = leftClosedPos;
        right.transform.localPosition = rightClosedPos;
        StartCoroutine(Open());
    }

    public void Load(string scene)
    {
        StartCoroutine(CloseAndLoad(scene));
    }

    IEnumerator CloseAndLoad(string scene)
    {
        while (Vector3.Distance(left.transform.localPosition, leftClosedPos) > 0.1f)
        {
            left.transform.localPosition = Vector3.MoveTowards(left.transform.localPosition, leftClosedPos, speed * Time.deltaTime);
            right.transform.localPosition = Vector3.MoveTowards(right.transform.localPosition, rightClosedPos, speed * Time.deltaTime);
            yield return null;
        }

        left.transform.localPosition = leftClosedPos;
        right.transform.localPosition = rightClosedPos;
        PlayerPrefs.Save();

        SceneManager.LoadScene(scene);
    }

    IEnumerator Open()
    {
        while (Vector3.Distance(left.transform.localPosition, leftOpenPos) > 0.1f)
        {
            left.transform.localPosition = Vector3.MoveTowards(left.transform.localPosition, leftOpenPos, speed * Time.deltaTime);
            right.transform.localPosition = Vector3.MoveTowards(right.transform.localPosition, rightOpenPos, speed * Time.deltaTime);
            yield return null;
        }

        left.transform.localPosition = leftOpenPos;
        right.transform.localPosition = rightOpenPos;
    }
}