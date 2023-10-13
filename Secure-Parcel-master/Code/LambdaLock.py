import json
import boto3
import os
from botocore.vendored import requests

TELE_TOKEN=os.environ['TOKEN']
URL = "https://api.telegram.org/bot{}/".format(TELE_TOKEN)
client = boto3.client('iot-data')

MyID = int(os.environ['id1'])
MyID2=int(os.environ['id2'])
myID3=int(os.environ['id3'])
def send_message(text, chat_id):
    if MyID == chat_id or MyID2==chat_id or myID3==chat_id:
        if text == 'unlock':
            final_text = 'Box is Unlocked'
            # Change topic, qos and payload
            response = client.publish(
            topic='topic/light',
            qos=0,
            payload=json.dumps(1)
            )
        
        elif text == 'lock':
            final_text = 'Box is locked'
            # Change topic, qos and payload
            response = client.publish(
            topic='topic/light',
            qos=0,
            payload=json.dumps(0)
            )
        elif text == 'storeface':
            final_text = 'face stored!'
            # Change topic, qos and payload
            response = client.publish(
            topic='topic/light',
            qos=0,
            payload=json.dumps(2)
            )
        
        else:
            final_text = 'Usage:\nType "unlock" to unlock the box\nType "lock" to lock the box\nType "storeface" to store your face to AI'
            
           
            
            
    else:
        final_text = 'You are not allowed to use this bot'
    url = URL + "sendMessage?text={}&chat_id={}".format(final_text, chat_id)
    requests.get(url)

def lambda_handler(event, context):
    message = json.loads(event['body'])
    chat_id = message['message']['chat']['id']                 
    reply = message['message']['text']
    send_message(reply, chat_id)
    
    

    return {
        'statusCode': 200
    }
