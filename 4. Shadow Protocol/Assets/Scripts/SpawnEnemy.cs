using System.Collections.Generic;
using UnityEngine;

public class SpawnEnemy : MonoBehaviour
{

    public GameObject[] Enems;

    public Transform[] points;

    List<Enemy> spawned = new List<Enemy>();

    public int maxPoints = 3;

    void Spawns()
    {

        for(int i = 0; i < Enems.Length; i++)
        {
            GameObject enem = Instantiate( Enems[ Random.Range(0, Enems.Length) ], points[Random.Range(0, points.Length)].position, Quaternion.identity, gameObject.transform);
            
            Enemy res;

            int rand = Random.Range(0, 3);
            res = enem.GetComponent<Enemy>();
            //if(rand == 0) res.enemytype = Enemy.type.nonActive; <-- мне лень делать анимацию Idle для енеми
            spawned.Add( res );
        }

        for(int i = 0; i < spawned.Count; i++)
        {
            List<Transform> addedPoints = new List<Transform>();
            int count = Random.Range(2, maxPoints+1);

            if(spawned[i].enemytype == Enemy.type.nonActive) count = 1;
            for (int j = 0; j < count; j++){
                Transform po;
                int tries = 20;


                do
                {
                    po = points[ Random.Range(0, points.Length) ];

                    if (addedPoints.Contains(po))
                        po = null;

                    else
                        addedPoints.Add(po);

                    tries--;
                } while(po == null && tries > 0);


                if(tries <= 0)
                    Debug.Log($"Не удалось заспавнить одного НПС {i} {j}");

                else
                    spawned[i].points.Add( po );
            }
            addedPoints.Clear();
        }


    }

    void Start()
    {
        Spawns();
    }

}
