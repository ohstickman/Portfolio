using UnityEngine;

public class MoneyDrop : MonoBehaviour
{
    public ParticleSystem parts;
    public Transform player;
    public float speed = 8f;
    public float wait = 1.3f;
    public int price = 1;
    public int parrts = 15;

    ParticleSystem.Particle[] particles;
    bool isLerp = false;

    void Start()
    {
        particles = new ParticleSystem.Particle[parts.main.maxParticles];
        player = GameObject.FindGameObjectWithTag("Player").transform;
    }

    void Update()
    {
        if (!isLerp || parts == null || player == null) return;
        
        int partsCount = parts.GetParticles(particles);
        Vector3 target = player.position;
        int newCount = 0;

        for (int i = 0; i < partsCount; i++)
        {
            if (particles[i].remainingLifetime <= 0f)
            {
                continue;
            }

            particles[i].position = Vector3.MoveTowards(particles[i].position, target, speed * Time.deltaTime);

            if (Vector3.Distance(particles[i].position, target) < 0.2f)
            {
                if (Statistic.Instance != null)
                {
                    Statistic.Instance.AddMoney(price / parrts);
                }
                continue; 
            }

            particles[newCount] = particles[i];
            newCount++;
        }

        if (newCount == 0)
        {
            parts.SetParticles(particles, newCount);
            isLerp = false;
            this.enabled = false;
            return;
        }

        parts.SetParticles(particles, newCount);
    }

    public void Drop()
    {
        parts.Play();
        Invoke("Activate", wait);
    }

    void Activate()
    {
        isLerp = true;
    }
}
