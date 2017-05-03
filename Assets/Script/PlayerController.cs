using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour {


	public float speed = 0.1f;
	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		Vector3 currentP = transform.position;
		float updown = Input.GetKey (KeyCode.DownArrow)?-speed:0;
		if (Input.GetKey (KeyCode.UpArrow)) {
			updown = speed;
		}
		currentP += new Vector3(Input.GetAxis("Horizontal"),updown,Input.GetAxis ("Vertical"))*speed;
		Quaternion q = transform.rotation;
		transform.position = currentP;
	}
}
