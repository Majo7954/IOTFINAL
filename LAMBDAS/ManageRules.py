import json
import boto3

client = boto3.client('iot-data', region_name='us-east-2')

def lambda_handler(event, context):
    try:
        # Extraer el estado de "fireDetected" del evento recibido
        reported_state = event.get('state', {}).get('reported', {})
        fire_detected = reported_state.get('fireDetected', False)
        alarm_active = reported_state.get('alarmActive', False)
        thing_name = event.get('thingName', 'ESP32')  # Asegúrate de recibir el nombre del dispositivo
        
        if fire_detected and not alarm_active:
            # Activar la alarma si se detecta fuego y aún no está activa
            print(f"Fuego detectado en {thing_name}. Activando alarma de emergencia.")
            payload = {
                "state": {
                    "desired": {
                        "alarmEnabled": True
                    }
                }
            }
        elif alarm_active:
            # Apagar la alarma después de que el evento ha sido procesado
            print(f"Apagando alarma en {thing_name} después de manejar el evento.")
            payload = {
                "state": {
                    "desired": {
                        "alarmEnabled": False,
                        "fireDetected": False
                    }
                }
            }
        else:
            # Si no hay fuego y la alarma no está activa, no hacer nada
            print(f"Sin fuego detectado en {thing_name}. No se requiere acción.")
            return {
                'statusCode': 200,
                'body': 'No se requiere acción.'
            }
        
        # Actualizar el shadow del dispositivo
        response = client.update_thing_shadow(
            thingName=thing_name,
            payload=json.dumps(payload)
        )
        print(f"Shadow actualizado con respuesta: {response}")
        
        return {
            'statusCode': 200,
            'body': 'Estado del dispositivo actualizado correctamente.'
        }
    except Exception as e:
        print(f"Error al procesar el evento: {e}")
        return {
            'statusCode': 500,
            'body': f"Error al procesar el evento: {str(e)}"
        }
