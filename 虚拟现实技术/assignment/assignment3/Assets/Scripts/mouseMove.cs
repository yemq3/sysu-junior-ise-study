using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class mouseMove : MonoBehaviour
{
    public GameObject player;    
    Vector3 rot = new Vector3(0, 0, 0);   
    public float speed; 
    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        float MouseX = Input.GetAxis("Mouse X")*speed;       
        float MouseY = Input.GetAxis("Mouse Y")*speed;
        rot.x = rot.x - MouseY;
        rot.y = rot.y + MouseX;  
        rot.z = 0;    
        if (rot.x<=-70) {
            transform.eulerAngles = new Vector3(-70, rot.y, 0);
        }
        else if (rot.x >= 11) {
            transform.eulerAngles = new Vector3(11, rot.y, 0);
        }
        else{
            transform.eulerAngles = new Vector3(rot.x, rot.y, 0);
        }
    }
}
