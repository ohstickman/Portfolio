using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class Combats : MonoBehaviour
{

    public float hitRadius = 15;
    public Animator anim;
    public LayerMask enemyLayer;


    public string[] combats;

    public float attackCooldown = 1f;
    float nextAttackTime = 0f;

    public float SecBeforeKill = 1;

    public static TextMeshProUGUI enems;

    void Start()
    {
        anim = GetComponent<Animator>();

        enems = GameObject.Find("EnemsCountTXT").GetComponent<TextMeshProUGUI>();

    }

    void Update()
    {

        bool isDie = PlayerController.Instance.Health == 0 ? true : false;

        if (Time.time < nextAttackTime || isDie) return;

        Collider[] hitColliders = Physics.OverlapSphere(transform.position, hitRadius, enemyLayer);

        List<Enemy> aliveEnemies = new List<Enemy>();



        foreach (Collider col in hitColliders)
        {
            Enemy enemy = col.GetComponent<Enemy>();

            if (enemy != null && enemy.isAlive)
            {
                aliveEnemies.Add(enemy);
            }
        }

        if (aliveEnemies.Count > 0)
        {

            nextAttackTime = Time.time + attackCooldown;

            int randomIndex = Random.Range(0, combats.Length);
            anim.SetTrigger(combats[randomIndex]);

            StartCoroutine(KillAnim(aliveEnemies));


        }
    }

    IEnumerator KillAnim(List<Enemy> aliveEnemies)
    {
        yield return new WaitForSeconds(SecBeforeKill);


        int count = 0;
        foreach (Enemy enemy in aliveEnemies)
        {
            enemy.Kill(transform.position);
            
            if(int.TryParse(enems.text, out count))
            {
                count--;
            }
            else
            {
                count = 0;
            } 
        }
        enems.text = count.ToString();
    }


    void OnDrawGizmosSelected()
    {
        Gizmos.color = Color.green;
        Gizmos.DrawWireSphere(transform.position, hitRadius);
    }

}
