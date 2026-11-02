using System;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class Upgrades : MonoBehaviour
{
    public Vector2 open;
    public Vector2 close;

    public RectTransform menu;
    public float speed = 10f;

    public Color havnotPrice;
    public Color havePrice;

    public Color haveMain;
    public Color havnotMain;

    public RectTransform play;
    public Vector2 op;
    public Vector2 cl;

    public Vector3 lvls;

    [Serializable]
    public class UpgradeUI
    {
        public Button button;
        public TextMeshProUGUI price;
        public Image back;
        public Image main;
        public TextMeshProUGUI lvl;
        public int pricem;
        public int maxLvl;
        public bool canUpgrade;
    }

    public UpgradeUI Speed;
    public UpgradeUI Inv;
    public UpgradeUI Com;

    Vector2 target;
    Vector2 targetP;

    void Start()
    {
        menu.anchoredPosition = close;
        play.anchoredPosition = cl;
        target = open;
        targetP = op;

        InitUI(Speed, 0);
        InitUI(Inv, 1);
        InitUI(Com, 2);
    }
    void InitUI(UpgradeUI ui, int type)
    {
        int currentLevel = type == 0 ? Statistic.Instance.SpeedLevel : type == 1 ? Statistic.Instance.InvLevel : Statistic.Instance.ComLevel;

        ui.lvl.text = "Ур " + currentLevel.ToString();
        ui.pricem = GetPrice(type, currentLevel);
        ui.price.text = ui.pricem.ToString();
        
        if (currentLevel >= ui.maxLvl) 
        {
            ui.button.interactable = false;
            ui.canUpgrade = true;
            ui.price.text = "Макс.";
        }
        else
        {
            ui.canUpgrade = false;
        }
    }

    public void StartGame()
    {
        PlayerController.Instance.isLock = false;

        target = close;
        targetP = cl;

    }

    int GetPrice(int type, int level)
    {
        if (type == 0) return Mathf.RoundToInt(120f * (Mathf.Pow(1.25f, level)));
        if (type == 1) return Mathf.RoundToInt(75f * (Mathf.Pow(1.2f, level)));
        return Mathf.RoundToInt(150f * (Mathf.Pow(1.3f, level)));
    }

    void Update()
    {
        if(Vector2.Distance(menu.anchoredPosition, target) >= 0.05f)
            menu.anchoredPosition = Vector2.Lerp(menu.anchoredPosition, target, speed * Time.deltaTime);

        if(Vector2.Distance(play.anchoredPosition, targetP) >= 0.05f)
            play.anchoredPosition = Vector2.Lerp(play.anchoredPosition, targetP, speed * Time.deltaTime);

        UpdateColors(Speed);
        UpdateColors(Inv);
        UpdateColors(Com);
    }

    void UpdateColors(UpgradeUI ui)
    {
        if (ui.canUpgrade) return;

        if(Statistic.Instance.money < ui.pricem)
        {
            ui.main.color = havnotMain;
            ui.back.color = havnotPrice;
        }
        else
        {
            ui.main.color = haveMain;
            ui.back.color = havePrice;
        }
    }

    public void Upgrade(int i)
    {
        PlayerPrefs.Save();
        UpgradeUI but = i == 0 ? Speed : i == 1 ? Inv : Com;

        if (but.canUpgrade) return;
        
        int currentLevel = i == 0 ? Statistic.Instance.SpeedLevel : i == 1 ? Statistic.Instance.InvLevel : Statistic.Instance.ComLevel;

        if (Statistic.Instance.money < but.pricem) return;

        if (!Statistic.Instance.RemoveMoney(but.pricem)) return;

        int newLvl = currentLevel + 1;
        
        if (i == 0) Statistic.Instance.UpdateSpeed(newLvl);
        else if (i == 1) Statistic.Instance.UpdateInv(newLvl);
        else Statistic.Instance.UpdateCom(newLvl);

        but.lvl.text = "Ур " + newLvl.ToString();
        but.pricem = GetPrice(i, newLvl);
        but.price.text = but.pricem.ToString();

        if (newLvl >= but.maxLvl)
        {
            but.button.interactable = false;
            but.price.text = "Макс.";
            but.canUpgrade = true;
        }
    }
}