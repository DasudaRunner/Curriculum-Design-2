# Curriculum-Design-2

##Description
    采用四路PWM控制四个地盘电机，电机驱动：L298n ，地盘电机：带减速箱的直流电机

    板2定时器0控制四个IO口发出PWM脉冲。
    
    改进：收到新的速度值时，经一系列计算后转换成PWM周期值，后对当前PWM周期与新周期值进行判断，若大于新设定值，则下一个PWM周期才更新周期值，反之，则不处理。防止速度突变。若控制周期很短时，次影响可忽略。
