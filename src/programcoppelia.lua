function CllbkVelComm(msg)    
    vo=1
    if(msg.data == 0)
    then
        vel_left = 1
        vel_right = 1
    end
    if(msg.data == 1)
    then
        vel_left = 0
        vel_right = 1
    end
    if(msg.data == 2)
    then
        vel_left = -1
        vel_right = -1
    end
    if(msg.data == 3)
    then
        vel_left = 1
        vel_right = 0
    end
    if(msg.data == 4)
    then
        vel_left = -1
        vel_right = 1
    end
    if(msg.data == 5)
    then
        vel_left = 1
        vel_right = -1
    end
    if(msg.data == 6)
    then
        v0 = 0
        vel_left = 0
        vel_right = 0
    end
    comm = msg
end
function sysCall_init() 
    local robot=sim.getObject('.')
    local obstacles=sim.createCollection(0)
    sim.addItemToCollection(obstacles,sim.handle_all,-1,0)
    sim.addItemToCollection(obstacles,sim.handle_tree,robot,1)
    usensors={}
    for i=1,16,1 do
        usensors[i]=sim.getObject("./ultrasonicSensor",{index=i-1})
        sim.setObjectInt32Param(usensors[i],sim.proxintparam_entity_to_detect,obstacles)
    end
    local v0=nil
    local comm = nil
    local vel_left = nil
    local vel_right = nil
    motorLeft=sim.getObject("./leftMotor")
    motorRight=sim.getObject("./rightMotor")
    noDetectionDist=0.5
    maxDetectionDist=0.2
    detect={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    braitenbergL={-0.2,-0.4,-0.6,-0.8,-1,-1.2,-1.4,-1.6, 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}
    braitenbergR={-1.6,-1.4,-1.2,-1,-0.8,-0.6,-0.4,-0.2, 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}        
    sub_vel_comm = simROS2.createSubscription('/pub_key_command', 'std_msgs/msg/Int32', 'CllbkVelComm')
    pub_sensor_data_4 = simROS2.createPublisher('/sensor_data_4', 'std_msgs/msg/Float32')
    pub_sensor_data_5 = simROS2.createPublisher('/sensor_data_5', 'std_msgs/msg/Float32')
end
-- This is a very simple EXAMPLE navigation program, which avoids obstacles using the Braitenberg algorithm


function sysCall_cleanup() 

end 

function sysCall_actuation() 
    local sensor_4, dist_4 = sim.readProximitySensor(usensors[4])
    local sensor_5, dist_5 = sim.readProximitySensor(usensors[5])
    data = {}
    data.distEmpat = dist_4 
    data.distLima = dist_5
    simROS2.publish(pub_sensor_data_4, {data=dist_4})
    simROS2.publish(pub_sensor_data_5, {data=dist_5})
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
    if(v0 == nil)
    then
        sim.setJointTargetVelocity(motorLeft,0)
        sim.setJointTargetVelocity(motorRight,0)
    else
        vLeft=vel_left
        vRight=vel_right
        
        for i=1,16,1 do
            vLeft=vLeft+braitenbergL[i]*detect[i]
            vRight=vRight+braitenbergR[i]*detect[i]
        end
        
        sim.setJointTargetVelocity(motorLeft,vLeft)
        sim.setJointTargetVelocity(motorRight,vRight)
    end
end 

