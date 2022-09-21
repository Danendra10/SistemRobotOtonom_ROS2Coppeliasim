unction sysCall_init() 
    if simROS then
        publisher=simROS.advertise('/Lspeed','std_msgs/Float32')
        publisher_sens=simROS.advertise('/sens','std_msgs/Float32MultiArray')
        subscriber1=simROS.subscribe('/sim_Lvel','std_msgs/Float32','subscriber_l_callback')
        subscriber2=simROS.subscribe('/sim_Rvel','std_msgs/Float32','subscriber_r_callback')
    end
    
    local robot=sim.getObject('.')
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
    
    ros_vLeft = 0
    ros_vRight = 0
end
-- This is a very simple EXAMPLE navigation program, which avoids obstacles using the Braitenberg algorithm


function sysCall_cleanup() 
 
end

function subscriber_l_callback(msg)
    -- This is the subscriber callback function
    ros_vLeft = msg.data
end

function subscriber_r_callback(msg)
    -- This is the subscriber callback function
    ros_vRight = msg.data
end

function sysCall_actuation() 
    local sens_pub = {}
    for i=1,16,1 do
        res,dist=sim.readProximitySensor(usensors[i])
        if(res == 0) then
           dist = 10 
        end
        table.insert(sens_pub, dist)
        if (res>0) and (dist<noDetectionDist) then
            if (dist<maxDetectionDist) then
                dist=maxDetectionDist
            end
            detect[i]=1-((dist-maxDetectionDist)/(noDetectionDist-maxDetectionDist))
        else
            detect[i]=0
        end
        
    end
    
    vLeft=v0
    vRight=v0
    
    for i=1,16,1 do
        vLeft=vLeft+braitenbergL[i]*detect[i]
        vRight=vRight+braitenbergR[i]*detect[i]
    end
    
    sim.setJointTargetVelocity(motorLeft,vLeft)
    sim.setJointTargetVelocity(motorRight,vRight)
    
    sim.setJointTargetVelocity(motorLeft,ros_vLeft)
    sim.setJointTargetVelocity(motorRight,ros_vRight)
    
    simROS.publish(publisher,{data=vLeft})
    simROS.publish(publisher_sens, {data=sens_pub})
end