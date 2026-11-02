using TMPro;
using UnityEngine;

public class Statistic : MonoBehaviour
{
    public static Statistic Instance;

    public float money = 0;
    public int MaxMoney = 999999;

    TextMeshProUGUI moneys;

    public int SpeedLevel = 0;
    public int InvLevel = 0;
    public int ComLevel = 0;

    public float defSpeed = 8;
    public float Defsecs => 2f;
    public float Defcool => 60f;

    float comRes = 1f;

    public int passed = 0;

    public TextMeshProUGUI pass;

    void Awake()
    {
        Instance = this;
        SpeedLevel = PlayerPrefs.GetInt("SpeedLevel", 0);
        InvLevel = PlayerPrefs.GetInt("InvLevel", 0);
        ComLevel = PlayerPrefs.GetInt("ComLevel", 0);
    }

    void Start()
    {
        money = PlayerPrefs.GetFloat("Moneys", 50f);
        passed = PlayerPrefs.GetInt("passed", 0);
        pass.text = "Пройдено: " + passed;

        if (Abilities.Instance != null)
            UpdateInv(InvLevel);

        if (PlayerController.Instance != null)
            UpdateSpeed(SpeedLevel);
        
        UpdateCom(ComLevel);
        FindText();
    }

    void FindText()
    {
        GameObject txt = GameObject.Find("MoneysCountTXT");
        if (txt != null)
        {
            moneys = txt.GetComponent<TextMeshProUGUI>();
            moneys.text = Mathf.FloorToInt(money).ToString();
        }
    }

    public void UpdateInv(int lvl)
    {
        InvLevel = lvl;
        PlayerPrefs.SetInt("InvLevel", lvl);

        float Rsecs = Defsecs * (1 + 0.1f * InvLevel);
        float Rcool = Defcool / (1 + 0.1f * InvLevel);
        
        if (Abilities.Instance != null)
        {
            Abilities.Instance.SecsUse = Rsecs;
            Abilities.Instance.SecsReload = Rcool;
        }
    }

    public void UpdateSpeed(int Speedl)
    {
        SpeedLevel = Speedl;
        PlayerPrefs.SetInt("SpeedLevel", SpeedLevel);

        float res = (float)(defSpeed + (defSpeed * SpeedLevel * 0.03));
        
        if (PlayerController.Instance != null)
        {
            PlayerController.Instance.speed = res;
        }
    }

    public void UpdateCom(int lvl)
    {
        ComLevel = lvl;
        PlayerPrefs.SetInt("ComLevel", ComLevel);
        comRes = 1f + (ComLevel * 0.1f);
    }

    public void AddMoney(float add)
    {
        money += (add * comRes);

        if (money > MaxMoney) money = MaxMoney;

        PlayerPrefs.SetFloat("Moneys", money);

        if (moneys != null) 
        {
            moneys.text = Mathf.FloorToInt(money).ToString();
        }
        else
        {
            FindText();
        }
    }

    public bool RemoveMoney(int price)
    {
        if (money - price < 0)
            return false;

        money -= price;
        PlayerPrefs.SetFloat("Moneys", money);

        if (moneys != null) 
        {
            moneys.text = Mathf.FloorToInt(money).ToString();
        }
        else
        {
            FindText();
        }
        return true;
    }

    void OnApplicationQuit()
    {
        PlayerPrefs.Save();
    }

    void OnDestroy()
    {
        PlayerPrefs.Save();
        if (Instance == this)
        {
            Instance = null;
        }
    }
}