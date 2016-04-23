<?php
    //error_reporting(E_ALL);
    //ini_set('display_errors', 1);
    include_once './config.inc.php';

    $inputPOST = file_get_contents("php://input");
    $input = json_decode($inputPOST);
    if ($input == null) {
        $reply['status'] = false;
        $reply['error'] = "No request";
        echo json_encode($reply);
        http_response_code(400);
        die();
    }


    

    $type = $input->type;

    $reply['status'] = true;
    $reply['type']= $type;
    if ($type == "all") {
        configureResponse($reply, "temperature");
        configureResponse($reply, "humidity");
        configureResponse($reply, "barometer");
        configureResponse($reply, "pressure");
    } elseif ($type == "temp") {
        configureResponse($reply, "temperature");
    } elseif ($type == "humd") {
        configureResponse($reply, "humidity");
    } elseif ($type == "baro") {
        configureResponse($reply, "barometer");
    } elseif ($type == "pres") {
        configureResponse($reply, "pressure");
    } else {
        $reply['status'] = false;
        $reply['error'] = "Invalid type: $type";
    }
    
    echo json_encode($reply);

    function configureResponse(&$reply, $var, $apiKey = PROTON_API, $deviceId = PROTON_ID) {
        $reply[substr($var, 0, 4)] = getIotState($deviceId, $apiKey, $var);
        if ($reply['status'] == true && $reply[substr($var, 0, 4)] == "-1") {
            $reply['status'] = false;
        }
    }

    function getIotState($deviceId, $apiKey, $state) {
        $url = "https://api.particle.io/v1/devices/$deviceId/$state?access_token=$apiKey";
        $curl = curl_init();
        curl_setopt($curl, CURLOPT_URL, $url);
        curl_setopt($curl,CURLOPT_CONNECTTIMEOUT,2);
        curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($curl, CURLOPT_SSL_VERIFYHOST, 0);
        curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, 0);
        $server_response = curl_exec($curl);
        curl_close($curl);
        
        if ($server_response) {
            $response = json_decode($server_response);
            if ($response->name == $state) {
                return $response->result;
            }
        }
        echo "$state failed\n";
        return "-1";
    }
?>