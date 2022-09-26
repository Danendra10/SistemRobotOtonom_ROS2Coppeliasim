function CllbkLeftMotor(msg)
    v_motor_left = msg.data
end

function CllbkRightMotor(msg)
    v_motor_right = msg.data
end

function sysCall_init() 
    if simROS2 then
        pub_posisi_x = simROS2.createPublisher("/posisi_x_robot", "std_msgs/msg/Float32")
        pub_posisi_z = simROS2.createPublisher("/posisi_z_robot", "std_msgs/msg/Float32")
        pub_posisi_x_target = simROS2.createPublisher("/posisi_x_target", "std_msgs/msg/Float32")
        pub_posisi_z_target = simROS2.createPublisher("/posisi_z_target", "std_msgs/msg/Float32")
        
        sub_left = simROS2.createSubscription("/v_left", "std_msgs/msg/Float32", "CllbkLeftMotor")
        sub_right = simROS2.createSubscription("/v_right", "std_msgs/msg/Float32", "CllbkRightMotor")
    end
    v_motor_left = 0
    v_motor_right = 0
    local robot=sim.getObject('.')
    local plane1 = sim.getObject('/Plane')
    local pos_robot = sim.getObjectPosition(robot, plane1)
    local pos_point = sim.getObjectPosition(plane1, robot)
    local obstacles=sim.createCollection(0)
    sim.addItemToCollection(obstacles,sim.handle_all,-1,0)
    sim.addItemToCollection(obstacles,sim.handle_tree,robot,1)
    usensors={}
    for i=1,16,1 do
        usensors[i]=sim.getObject("./ultrasonicSensor",{index=i-1})
        sim.setObjectInt32Param(usensors[i],sim.proxintparam_entity_to_detect,obstacles)
    end
    
    motorLeft=sim.getObject("./leftMotor")
    motorRight=sim.getObject("./rightMotor")
    noDetectionDist=0.5
    maxDetectionDist=0.2
    detect={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    braitenbergL={-0.2,-0.4,-0.6,-0.8,-1,-1.2,-1.4,-1.6, 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}
    braitenbergR={-1.6,-1.4,-1.2,-1,-0.8,-0.6,-0.4,-0.2, 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}
    v0=2
end
-- This is a very simple EXAMPLE navigation program, which avoids obstacles using the Braitenberg algorithm


function sysCall_cleanup() 
 
end 

function sysCall_actuation() 
    for i=1,16,1 do
        res,dist=sim.readProximitySensor(usensors[i])
        if (res>0) and (dist<noDetectionDist) then
            if (dist<maxDetectionDist) then
                dist=maxDetectionDist
            end
            detect[i]=1-((dist-maxDetectionDist)/(noDetectionDist-maxDetectionDist))
        else
            detect[i]=0
        end
    end
    
    --for i=1,16,1 do
        --vLeft=vLeft+braitenbergL[i]*detect[i]
        --vRight=vRight+braitenbergR[i]*detect[i]
    --end
    
    vLeft=v_motor_left
    vRight=v_motor_right
    
    --sim.getJointTargetVelocity(motorLeft, vLeft)
    --sim.getJointTargetVelocity(motorRight, vRight)
    
    sim.setJointTargetVelocity(motorLeft,vLeft)
    sim.setJointTargetVelocity(motorRight,vRight)    
    
    simROS2.publish(pub_posisi_x, {data=pos_robot[1]})
    
end 
