using System.Collections;
using NUnit.Framework.Constraints;
using TMPro;
using UnityEngine;

public class TestTerminal : MonoBehaviour
{
    
    public float Radius = 10f;

    Transform Player;

    public bool isTrigged = false;

    TextMeshProUGUI enems;

    TextMeshProUGUI info;
    Transform parent;
    PlayerController pc;

    void Start()
    {
        Player = GameObject.FindGameObjectWithTag("Player").transform;
        enems = Combats.enems;
    }

    void Update()
    {
        if(Vector3.Distance(Player.position, transform.position) <= Radius && !isTrigged)
        {
            int count = 0;
            if(int.TryParse(enems.text, out count))
            {
                if(count <= 0)
                {
                    isTrigged = true;
                    int pass = ++Statistic.Instance.passed;
                    PlayerPrefs.SetInt("passed", pass);
                    SceneReloader.Instance.Load(UnityEngine.SceneManagement.SceneManager.GetActiveScene().name);
                }

                else
                {
                    PlayerController.Instance.msg("Зачисти локацию!");
                }
            }
            else
            {
                Debug.Log("Err");
            } 


        }
    }


    void OnDrawGizmosSelected() {
        Gizmos.color = Color.white;
        Gizmos.DrawWireSphere(transform.position, Radius);    
    }
}
