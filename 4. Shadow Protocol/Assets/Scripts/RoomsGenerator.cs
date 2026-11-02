using TMPro;
using Unity.AI.Navigation;
using UnityEngine;

public class RoomsGenerator : MonoBehaviour
{

    public GameObject[] rooms;

    public int Count = 2;

    public Transform fstPos;

    public GameObject lastRoom;

    TextMeshProUGUI enems;

    public int each = 3;

    int enemies = 0;

    void Generate()
    {

        int bonusRooms = Statistic.Instance.passed / 3;

        Count += bonusRooms;

        Transform exit = fstPos;

        for(int i = 0; i < Count; i++)
        {
            GameObject roomId = rooms[Random.Range(0, rooms.Length)];

            GameObject room = Instantiate(roomId, exit.position, exit.rotation);

            SpawnEnemy se = room.GetComponent<SpawnEnemy>();

            enemies += se.Enems.Length;

            exit = room.transform.Find("Exit");
        }

        GameObject lroom = Instantiate(lastRoom, exit.position, exit.rotation);
        SpawnEnemy lse = lroom.GetComponent<SpawnEnemy>();
        enemies += lse.Enems.Length;

        enems.text = enemies.ToString();

    }

    void Start()
    {
        enems = GameObject.Find("EnemsCountTXT").GetComponent<TextMeshProUGUI>();
        Generate();

    }

}
