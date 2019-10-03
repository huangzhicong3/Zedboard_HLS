

// 640 X 480 @ 60Hz with a 25.000MHz pixel clock

`define H_ACTIVE		640
`define H_FRONT_PORCH	16
`define H_SYNCH			96
`define H_BACK_PORCH	48
`define H_TOTAL			( `H_SYNCH + `H_BACK_PORCH + `H_ACTIVE + `H_FRONT_PORCH ) 
//640 // pixels

`define V_ACTIVE		480	
`define V_FRONT_PORCH  11
`define V_SYNCH			2
`define V_BACK_PORCH	31	
`define V_TOTAL			 (`V_SYNCH + `V_BACK_PORCH +  `V_ACTIVE +`V_FRONT_PORCH )	
//480 // lines

module my_vga_syn (
        input pixel_clock,	rst_i,
        output  reg 	h_synch,	v_synch,
        output reg 	dp_en,
        output reg  [10:0]	line_count  ,
        output reg  [10:0]  pixel_count
    );

    /*
      1),做2个计数器，一个用来记录行内的像素点时钟数，
      一个用来记录行数。
      这两个计数器除了复位信号外不受别的因素影响，
      在一直自己循环累加。
    */

    always @(posedge pixel_clock)
        if ( rst_i )pixel_count<=0;
        else if (pixel_count == (`H_TOTAL-1))pixel_count<=0;
        else pixel_count<=pixel_count+1;

    always @(posedge pixel_clock)
        if (rst_i)line_count<=0;
        else if (pixel_count==(`H_TOTAL-1)) begin

            if ( line_count==(`V_TOTAL-1))line_count<=0;
            else line_count<=line_count+1;

        end

    /*
    2）,两个计数器的数值就对应上图的所处的位置。
    我们找到实际要显示区域对应的像素计数器和行计数器
    的数值，只要两个计数器满足在这个区域，
    就允许显示，否则进行消音输出（R,G,B都输出为0）。
    */
    reg h_en  ;
    always@(posedge pixel_clock)
        if (pixel_count == (`H_TOTAL-1) )h_en<=1;
        else if (pixel_count==(`H_ACTIVE-1))h_en<=0;

    reg v_en ;
    always@(posedge pixel_clock)
        if (pixel_count == (`H_TOTAL-1) )begin

            if (line_count==(`V_TOTAL-1))v_en<=1;
            else if (line_count==(`V_ACTIVE-1))v_en<=0;

        end

    always@(posedge pixel_clock) dp_en <= v_en & h_en ;



    //3）,从图中找到H_SYN的时间段和V_SYN的时间段，
    //将这个时间段同步信号对应设置为低电平。

    always@(posedge pixel_clock)
        if ( pixel_count ==(`H_ACTIVE+`H_FRONT_PORCH-1))
            h_synch<=0;
        else if ( pixel_count ==(`H_ACTIVE+`H_FRONT_PORCH+`H_SYNCH-1))
            h_synch<=1;

    always@(posedge pixel_clock)
        if ( pixel_count ==(`H_TOTAL-1))

        begin
            if (line_count==(`V_ACTIVE+`V_FRONT_PORCH-1))
                v_synch<=0;
            else if ( line_count ==(`V_ACTIVE+`V_FRONT_PORCH + `V_SYNCH-1))
                v_synch<=1;

        end




endmodule
