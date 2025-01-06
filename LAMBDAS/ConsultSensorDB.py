import boto3
import json
from boto3.dynamodb.conditions import Key
from decimal import Decimal

dynamodb = boto3.resource('dynamodb')
table = dynamodb.Table('SensorData')

# Conversión de Decimal a tipos serializables
def decimal_default(obj):
    if isinstance(obj, Decimal):
        return float(obj)
    raise TypeError

def lambda_handler(event, context):
    try:
        sensor_id = event['SensorID']
        start_time = event['StartTime']
        end_time = event['EndTime']

        # Consulta en DynamoDB
        response = table.query(
            KeyConditionExpression=Key('SensorID').eq(sensor_id) & Key('Timestamp').between(start_time, end_time)
        )
        items = response['Items']

        # Serializa los datos convirtiendo Decimals
        return {
            'statusCode': 200,
            'body': json.dumps(items, default=decimal_default)  # Conversión aquí
        }

    except Exception as e:
        return {
            'statusCode': 500,
            'body': f"Error al consultar datos: {str(e)}"
        }
