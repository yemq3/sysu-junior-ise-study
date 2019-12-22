using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class move : MonoBehaviour
{
    CharacterController characterController;

    public float speed = 6.0f;

    public Rigidbody rg;

    // Start is called before the first frame update
    void Start()
    {
        rg = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update()
    {
        float MoveX = Input.GetAxis("Horizontal");
        float MoveY = Input.GetAxis("Vertical");
        transform.Translate(new Vector3(MoveX, 0, MoveY) *
            Time.deltaTime * speed);
        if (Input.GetButtonDown("Jump")) {
            rg.AddForce(new Vector3(0, 5f, 0), ForceMode.Impulse);
        }
    }
}
