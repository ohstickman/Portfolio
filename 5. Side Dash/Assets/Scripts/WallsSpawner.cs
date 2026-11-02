using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class WallsSpawner : MonoBehaviour
{
    public GameObject[] walls;
    public Transform[] points;
    public float speed = 8f;
    public float speedMulti = 1.05f;

    public TextMeshProUGUI t;
    public TextMeshProUGUI maxt;
    public int maxf = 0;
    
    
    List<GameObject> spawned = new List<GameObject>();

    int active = 0;
    public int max = 1;

    int tryi = 0;


    List<Transform> locked = new List<Transform>();

    void Start()
    {
        maxf = PlayerPrefs.GetInt("max", 0);
        maxt.text = maxf.ToString();
        t.text = tryi.ToString();
    }

    void Update()
    {
        if (active < max)
        {
            Transform p;
            do{ p = points[Random.Range(0, points.Length)];} while(locked.Contains(p));
            locked.Add(p);
            GameObject w = walls[Random.Range(0, walls.Length)];
            spawned.Add(Instantiate(w, p.position, p.rotation));
            active++;
        }

        for (int i = spawned.Count - 1; i >= 0; i--)
        {
            GameObject c = spawned[i];
            c.transform.position = Vector3.MoveTowards(c.transform.position, Vector3.zero, speed * Time.deltaTime);


            if (Vector3.Distance(c.transform.position, Vector3.zero) < 0.5f)
            {
                active--;
                Destroy(c);
                speed *= speedMulti;
                speed = Mathf.Min(speed, 20f);
                tryi++;
                t.text = tryi.ToString();
                if(maxf < tryi)
                {
                    maxt.text = tryi.ToString();
                }
                PlayerPrefs.SetInt("tryi", tryi);
                PlayerPrefs.SetInt("max", maxf);
                if(tryi == 3|| tryi == 10 || tryi == 20 || tryi == 60) max++;
                spawned.RemoveAt(i); 
                locked.Clear();
            }
        }
    }

    public void Restart()
    {
        SceneManager.LoadScene(0);
    }

    void OnApplicationQuit()
    {
        PlayerPrefs.Save();
    }
}