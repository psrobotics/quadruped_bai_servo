# Quadruped_bai_servo v1
<img src="pic\quad_servo_cad.jpg" alt="quad_servo_cad" style="zoom:50%;" />

A low cost 12DOF quadruped, actuated by hobby servos. Notice that this is a early version developed back in 2019. If you're looking for a high performance desktop quad, check out the ongoing [Quadruped bai tiny](https://github.com/psrobotics/Quadruped_bai_tiny)

## Videos

[Balancing](https://www.youtube.com/watch?v=fsc0w1NeIVk)

[Walking](https://youtu.be/FCRvykWv6ok)

## Hardware

Required parts are listed below. For all printed parts, the recommended printing infill is 70%. Set up proper printing direction and support based on your printer type.

| Part Name                                                   | Quantity | Link                                                         | Info                |
| ----------------------------------------------------------- | -------- | ------------------------------------------------------------ | ------------------- |
| leg_tip                                                     | 2        | [STL file](hardware/stl)                                     | Printed part        |
| leg_tip_MIR                                                 | 2        | /                                                            | /                   |
| leg_2                                                       | 2        | /                                                            | /                   |
| leg_2_MIR                                                   | 2        | /                                                            | /                   |
| leg_link_1                                                  | 2        | /                                                            | /                   |
| leg_link_1_MIR                                              | 2        | /                                                            | /                   |
| leg_link-1                                                  | 2        | /                                                            | /                   |
| leg_link-1_MIR                                              | 2        | /                                                            | /                   |
| leg_1                                                       | 2        | /                                                            | /                   |
| leg_1_MIR                                                   | 2        | /                                                            | /                   |
| leg_1_2                                                     | 2        | /                                                            | /                   |
| leg_1_2_MIR                                                 | 2        | /                                                            | /                   |
| servo_holder_1                                              | 2        | /                                                            | /                   |
| servo_holder_1_MIR                                          | 2        | /                                                            | /                   |
| hip_joint_holder_1                                          | 2        | /                                                            | /                   |
| hip_joint_holder_1_MIR                                      | 2        | /                                                            | /                   |
| hip_holder_2                                                | 2        | /                                                            | /                   |
| hip_holder_2_MIR                                            | 2        | /                                                            | /                   |
| joint_ab_holder                                             | 2        | /                                                            | /                   |
| body_bridge                                                 | 2        | /                                                            | /                   |
| body_frame_2                                                | 1        | /                                                            | /                   |
| body_frame_1                                                | 1        | /                                                            | /                   |
| pcb_holder                                                  | 2        | /                                                            | /                   |
| M3*8                                                        | ~20      | /                                                            | Screw               |
| M3*12                                                       | ~30      | /                                                            | /                   |
| M3*16                                                       | ~20      | /                                                            | /                   |
| M2*8                                                        | ~50      | /                                                            | /                   |
| M2*15                                                       | ~50      | /                                                            | /                   |
| M2*20                                                       | ~20      | /                                                            | /                   |
| M2*26                                                       | ~20      | /                                                            | /                   |
| SF-1 0305 Self Lubricating Composite Bearing Bushing Sleeve | ~25      | [ebay](https://www.ebay.com/itm/141882850401?_trkparms=amclksrc%3DITM%26aid%3D1110006%26algo%3DHOMESPLICE.SIM%26ao%3D1%26asc%3D20200818143230%26meid%3Df2f23010710f47e39153549f7afe6b5b%26pid%3D101224%26rk%3D2%26rkt%3D5%26sd%3D141882852158%26itm%3D141882850401%26pmt%3D0%26noa%3D1%26pg%3D2047675%26algv%3DDefaultOrganicWeb%26brand%3DUnbranded&_trksid=p2047675.c101224.m-1) | For rotation joints |
| EMAX ES08A II Servo                                         | 12       | [EMAX](https://emaxmodel.com/products/emax-es08a-ii-mini-plastic-gear-analog-servo-1-8kg-sec-for-rc-models-robot) | Actuator            |

After printing your parts, insert the bearing at all 5mm holes locates around rotations joints. Then fasten  parts with M3 screws. Remember to check the rotation friction, make sure all joints can rotates freely with the servo installed. 

The robot consists 4 legs and a body frame. Assembly 2 legs first.

![leg_ass_1](pic/leg_ass_1.jpg)

Assembly another 2 mirrored legs. Then link all legs to the body frame.

<img src="pic/body_ass_1.jpg" alt="body_ass_1" style="zoom:67%;" />
