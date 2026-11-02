using System.Collections;
using UnityEngine;

public class Grid : MonoBehaviour
{
    public Material mat;
    public Renderer mesh;

    public Color[] pulses;
    public Color kill;
    public Color die;

    public float speedPuls = 2f;
    public float speedEx = 5f;
    public float iNspeedEx = 20f;

    public static Grid Instance;

    private int i = 0;
    private bool isEx = false;
    private Color targetColor;

    public void Start()
    {
        Instance = this;
        mat = mesh.material;
        mat.color = pulses[0];
        targetColor = pulses[0];
    }

    bool IsClose(Color a, Color b, float threshold)
    {
        float diffR = Mathf.Abs(a.r - b.r);
        float diffG = Mathf.Abs(a.g - b.g);
        float diffB = Mathf.Abs(a.b - b.b);
        return (diffR + diffG + diffB) < threshold;
    }
    int b = 0;
    void Update()
    {
        if(IsClose(mat.color, die, 0.05f)) return;
        if (isEx)
        {
            
            if(b == 0) mat.color = Color.Lerp(mat.color, targetColor, iNspeedEx * Time.deltaTime);
            if(b == 1) mat.color = Color.Lerp(mat.color, targetColor, speedEx * Time.deltaTime);
            
            if (IsClose(mat.color, targetColor, 0.05f))
            {
                if(b == 0){ targetColor = pulses[i]; b = 1;}
                else{ isEx = false; b = 0; targetColor = pulses[i];}
            }
            return; 
        }

        mat.color = Color.Lerp(mat.color, targetColor, speedPuls * Time.deltaTime);

        if (IsClose(mat.color, targetColor, 0.05f))
        {
            i++;
            if (i >= pulses.Length) i = 0; 
            targetColor = pulses[i];
        }
    }

    public void Ex(int i)
    {
        isEx = true;         
        targetColor = i == 0 ? die : kill;     
    }
}