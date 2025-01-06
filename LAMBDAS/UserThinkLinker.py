import boto3
import json

dynamodb = boto3.resource('dynamodb')
table = dynamodb.Table('UserThings')

def lambda_handler(event, context):
    try:
        user_id = event['UserID']
        thing_name = event['ThingName']

        table.put_item(
            Item={
                'UserID': user_id,
                'ThingName': thing_name
            }
        )
        return {
            'statusCode': 200,
            'body': json.dumps(f"Usuario {user_id} vinculado al dispositivo {thing_name}.")
        }
    except Exception as e:
        return {
            'statusCode': 500,
            'body': json.dumps(f"Error al vincular usuario: {str(e)}")
        }
