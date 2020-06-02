using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Destroy : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        Destroy (gameObject,5);
    }

    void OnCollisionEnter(Collision other) 
    {
        if (other.gameObject.CompareTag ("goals"))
        {
            other.gameObject.SetActive (false);
            Destroy(gameObject);
        }
    }
}
