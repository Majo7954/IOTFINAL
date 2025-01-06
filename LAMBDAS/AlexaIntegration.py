import boto3
import json

iot_data_client = boto3.client('iot-data', region_name='us-east-2')

def update_shadow(thing_name, state):
    try:
        payload = json.dumps({"state": {"desired": state}})
        response = iot_data_client.update_thing_shadow(
            thingName=thing_name,
            payload=payload
        )
        print(f"Shadow actualizado correctamente: {response}")
        return True
    except Exception as e:
        print(f"Error al actualizar el shadow: {e}")
        return False

def get_shadow(thing_name):
    try:
        response = iot_data_client.get_thing_shadow(thingName=thing_name)
        shadow = json.loads(response['payload'].read())
        print(f"Estado actual del shadow: {shadow}")
        return shadow
    except Exception as e:
        print(f"Error al obtener el estado del shadow: {e}")
        return None

def lambda_handler(event, context):
    request_type = event['request']['type']

    if request_type == 'LaunchRequest':
        speak_output = 'Bienvenido al sistema de invernadero inteligente. Puedes activar o desactivar la alarma de emergencia o consultar el estado actual. ¿Qué deseas hacer?'
        return build_response(speak_output)

    elif request_type == 'IntentRequest':
        intent_name = event['request']['intent']['name']

        if intent_name == 'TurnOnEmergencyIntent':
            if update_shadow('ESP32', {'alarmEnabled': True}):
                speak_output = 'EMERGENCIA: La alarma del invernadero ha sido activada. Verifica el estado del sistema inmediatamente.'
            else:
                speak_output = 'Hubo un error al intentar activar la alarma de emergencia.'

        elif intent_name == 'TurnOffEmergencyIntent':
            if update_shadow('ESP32', {'alarmEnabled': False}):
                speak_output = 'La alarma del invernadero ha sido desactivada. Todo está bajo control.'
            else:
                speak_output = 'Hubo un error al intentar desactivar la alarma de emergencia.'

        elif intent_name == 'CheckEmergencyStatusIntent':
            shadow = get_shadow('ESP32')
            if shadow and 'state' in shadow and 'reported' in shadow['state']:
                alarm_enabled = shadow['state']['reported'].get('alarmEnabled', False)
                if alarm_enabled:
                    speak_output = 'EMERGENCIA: La alarma del invernadero está activada. Verifica el sistema inmediatamente.'
                else:
                    speak_output = 'La alarma del invernadero está desactivada. Todo está funcionando normalmente.'
            else:
                speak_output = 'Error al consultar el estado del sistema.'

        else:
            speak_output = 'Lo siento, no reconozco esa solicitud.'

        return build_response(speak_output)

    else:
        return build_response('Tipo de solicitud no soportado.')

def build_response(speak_output):
    return {
        'version': '1.0',
        'response': {
            'outputSpeech': {
                'type': 'PlainText',
                'text': speak_output
            },
            'shouldEndSession': False
        }
    }
