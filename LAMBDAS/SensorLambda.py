import json
import boto3
from botocore.exceptions import ClientError
from datetime import datetime

dynamodb = boto3.resource('dynamodb')
table = dynamodb.Table('SensorData')

def lambda_handler(event, context):
    try:

        print(f"Evento recibido: {json.dumps(event)}")
        
        payload = event['Records'][0]['Sns']['Message']
        shadow_data = json.loads(payload)
        
        thing_name = event['Records'][0]['Sns']['Subject']
        
        reported_state = shadow_data.get('state', {}).get('reported', {})
        
        temperature = reported_state.get('temperature', None)
        humidity = reported_state.get('humidity', None)
        soil_moisture = reported_state.get('sensors', {}).get('soilMoisture', None)
        air_quality = reported_state.get('sensors', {}).get('airQualityState', None)
        light_detected = reported_state.get('sensors', {}).get('lightDetected', None)
        
        timestamp = str(datetime.utcnow())
        
        item = {
            'ThingName': ESP32,
            'Timestamp': timestamp, 
            'Temperature': temperature,
            'Humidity': humidity,
            'SoilMoisture': soil_moisture,
            'AirQuality': air_quality,
            'LightDetected': light_detected
        }
        
        response = table.put_item(Item=item)
        
        # Log para verificar que los datos fueron guardados correctamente
        print(f"Datos guardados en DynamoDB: {json.dumps(item)}")
        
        return {
            'statusCode': 200,
            'body': json.dumps('Datos de los sensores guardados correctamente en DynamoDB')
        }
    
    except ClientError as e:
        print(f"Error al guardar los datos en DynamoDB: {str(e)}")
        return {
            'statusCode': 500,
            'body': json.dumps(f'Error al guardar los datos: {str(e)}')
        }
