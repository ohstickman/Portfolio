using UnityEngine;

public class SaveAndroid : MonoBehaviour
{
    void Start()
    {
        QualitySettings.vSyncCount = 1; 
        Application.targetFrameRate = 60; 
    }
}
