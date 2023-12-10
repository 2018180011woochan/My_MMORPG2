using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectManager
{
    // 나중에 서버에선 id를 추가해서 딕셔너리로
    //Dictionary<int, GameObject> _objects = new Dictionary<int, GameObject> ();
    List<GameObject> _objectes = new List<GameObject>();
    
    public void Add(GameObject go)
    {
        _objectes.Add(go);
    }

    public void Remove(GameObject go)
    {
        _objectes.Remove(go);
    }

    // 부하가 엄청나!
    public GameObject Find(Vector3Int cellPos)
    {
        foreach (GameObject obj in _objectes)
        {
            CreatureController cc = obj.GetComponent<CreatureController>();
            if (cc == null) continue;

            if (cc.CellPos == cellPos)
                return obj;
        }

        return null;
    }

    public GameObject Find(Func<GameObject, bool> condition)
    {
        foreach (GameObject obj in _objectes)
        {
            if (condition.Invoke(obj))
                return obj;
        }

        return null;
    }

    public void Clear()
    {
        _objectes.Clear();
    }
}
