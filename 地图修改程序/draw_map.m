function draw_map
clc;
clear all;
close all;
global select;
global select_reset;
global selectr;
global select_id;
global N;
global id;
global qrx;
global qry;
global qryaw;
global MARK_L;
global read_num;
MARK_L=0.18;%/m
read_num=51;
figure;
fig = figure;
set(gcf,'WindowButtonDownFcn',@ButttonDownFcn);
set(fig,'windowkeypressfcn',@keypressfcn);
set(fig,'windowkeyreleasefcn',@keyreleasefcn);
    function keypressfcn(h,evt)
        if(evt.Key=='a')
            s=-5;d=-5;
        end  
        if(evt.Key=='d')
            s=5;d=5;
        end
        if(evt.Key=='w')
            s=-45;
        end  
        if(evt.Key=='s')
            s=45;
        end  
       % fprintf('%d',select_reset);
        if select_reset~=0 %rotate coordinate
            yaw_off=d;
            cy=cos(-yaw_off*0.0173);	
            sy=sin(-yaw_off*0.0173);	
            for i=1:N
            qrx(i)=qrx(i)*cy - qry(i)*sy;
            qry(i)=qrx(i)*sy + qry(i)*cy;
            qryaw(i)=To_180_degrees(qryaw(i)+s);
            end
            fprintf('Rotate Coordinate\n');
            clf; 
            for i=1:N
            pos(1)=qrx(i);
            pos(2)=qry(i);
            pos(3)=0;
            pos(4)=qryaw(i);
            draw_mark(pos,MARK_L,2,'-k');
            str = [' ',num2str(id(i))];
            text(pos(1)+MARK_L/2,pos(2)+MARK_L/2,str,'Color','blue','FontSize',10);
            hold on;
            grid on;
            axis equal;
            title('Redrawed Marker ');
            end 
            ylim=get(gca,'Ylim'); % 获取当前图形的纵轴的范围
            hold on
            plot([0,0],ylim,'m--','LineWidth',2); % 绘制x=1的直线
            xlim=get(gca,'Xlim'); % 获取当前图形的纵轴的范围
            hold on
            plot(xlim,[0,0],'m--','LineWidth',2); % 绘制x=1的直线
            xlabel('Axis-X(m)')
            ylabel('Axis-Y(m)')
        end
        if select_id~=0  %rotate on mark
            qryaw(select_id)=To_180_degrees(qryaw(select_id)+s);
            fprintf('Redraw Marker %d Yaw: %f\n',select_id,qryaw(select_id));
            clf; 
            for i=1:N
            pos(1)=qrx(i);
            pos(2)=qry(i);
            pos(3)=0;
            pos(4)=qryaw(i);
            draw_mark(pos,MARK_L,2,'-k');
            str = [' ',num2str(id(i))];
            text(pos(1)+MARK_L/2,pos(2)+MARK_L/2,str,'Color','blue','FontSize',10);
            hold on;
            grid on;
            axis equal;
            title('Redrawed Marker ');
            end 
            ylim=get(gca,'Ylim'); % 获取当前图形的纵轴的范围
            hold on
            plot([0,0],ylim,'m--','LineWidth',2); % 绘制x=1的直线
            xlim=get(gca,'Xlim'); % 获取当前图形的纵轴的范围
            hold on
            plot(xlim,[0,0],'m--','LineWidth',2); % 绘制x=1的直线
            xlabel('Axis-X(m)')
            ylabel('Axis-Y(m)')
        end
    end
    function keyreleasefcn(h,evt)
        if(evt.Key=='a')
        end
    end

end

function ButttonDownFcn(src,event)
persistent init;
global select;
global select_reset;
global selectr;
global select_id;
global N;
global id;
global qrx;
global qry;
global qryaw;
global MARK_L;
global read_num;

if isempty(selectr)
    selectr=1;
end
if isempty(select_reset)
    select_reset=1;
end
if isempty(select)
    select=0;
end
if isempty(select_id)
    select_id=0;
end
if isempty(init)
    init=1;
    str1=sprintf('./MAP/%d.txt',read_num);
    fprintf('str1');
    SD1=load(str1); 
    id=SD1(:,1);
    [length,~]=size(id);
    N=length;
    cnt=2;
    size_am=0.001;
    size_am=1;
    qrx=SD1(:,cnt)*size_am;cnt=cnt+1; qry=SD1(:,cnt)*size_am;cnt=cnt+1; qrz=SD1(:,cnt)*size_am;cnt=cnt+1; qryaw=SD1(:,cnt);cnt=cnt+1;
    for i=1:N
    pos(1)=qrx(i);
    pos(2)=qry(i);
    pos(3)=0;
    pos(4)=qryaw(i);
    draw_mark(pos,MARK_L,2,'-k');
    str = [' ',num2str(id(i))];
    text(pos(1)+MARK_L/2,pos(2)+MARK_L/2,str,'Color','blue','FontSize',10);
    hold on;
    grid on;
    axis equal;
    end
    ylim=get(gca,'Ylim'); % 获取当前图形的纵轴的范围
    hold on
    plot([0,0],ylim,'m--','LineWidth',2); % 绘制x=1的直线
    xlim=get(gca,'Xlim'); % 获取当前图形的纵轴的范围
    hold on
    plot(xlim,[0,0],'m--','LineWidth',2); % 绘制x=1的直线
    xlabel('Axis-X(m)')
    ylabel('Axis-Y(m)')
end    

if(selectr==1)% init finish 
        mid=0;
        if strcmp(get(gcf,'SelectionType'),'alt')% 此时即为右键
        mid=2;
        elseif strcmp(get(gcf,'SelectionType'),'normal')% 此时即为左键
        mid=1;
        end
        [pt] = get(gca,'CurrentPoint');
        x = pt(1,1);
        y = pt(1,2);
        %find the closet mark
        min_id=0;
        dis_min=10000;
         for i=1:N
            pos(1)=qrx(i);
            pos(2)=qry(i);
            dis=((pos(1)-x)^2+(pos(2)-y)^2)^0.5;
            if(dis<dis_min)
               dis_min=dis;
               min_id=i;
            end    
         end
        
         %redraw
         if(select==1&&mid==1)
            select=0; 
            if select_reset==1
                select_reset=0;
            end
            qrx(select_id)=x;
            qry(select_id)=y;
            fprintf('Redraw Marker %d at x:%f y:%f\n',id(select_id),x,y);
            clf; 
            for i=1:N
            pos(1)=qrx(i);
            pos(2)=qry(i);
            pos(3)=0;
            pos(4)=qryaw(i);
            draw_mark(pos,MARK_L,2,'-k');
            str = [' ',num2str(id(i))];
            text(pos(1)+MARK_L/2,pos(2)+MARK_L/2,str,'Color','blue','FontSize',10);
            hold on;
            grid on;
            axis equal;
            title('Redrawed Marker ');
            end 
            ylim=get(gca,'Ylim'); % 获取当前图形的纵轴的范围
            hold on
            plot([0,0],ylim,'m--','LineWidth',2); % 绘制x=1的直线
            xlim=get(gca,'Xlim'); % 获取当前图形的纵轴的范围
            hold on
            plot(xlim,[0,0],'m--','LineWidth',2); % 绘制x=1的直线
            xlabel('Axis-X(m)')
            ylabel('Axis-Y(m)')
         end  

          if(dis_min<0.02&&select==0&&mid==1)
            select_id=min_id;
            select=1;
            fprintf('Select Marker %d\n',id(select_id));
             title('Selected Marker');
          end   
       
          %reset coordinate
            %saving
          c = get(gcf,'Currentcharacter');
          val = double(c);
          if(mid==0&&select_reset==1&&select==0)
            select_reset=2;
            title('Saving Map');
            str1=sprintf('./MAP_C/%d.txt',read_num);
            fp=fopen(str1,'a');
            for i=1:N
            pos(1)=qrx(i);
            pos(2)=qry(i);
            pos(3)=0;
            pos(4)=qryaw(i);
            fprintf(fp,'%d %f %f %f %f\r\n',id(i),pos(1),pos(2),pos(3),pos(4));
            end 
            fclose(fp);
          end  
           
          
          if(mid==2&&select_reset==1&&select==0)
            select_reset=2;
            min_id0=1;
            dis_min0=10000;
             for i=1:N
                pos(1)=qrx(i);
                pos(2)=qry(i);
                dis0=((pos(1))^2+(pos(2))^2)^0.5;
                if(dis0<dis_min0)
                   dis_min0=dis0;
                   min_id0=i;
                end    
             end
            offx=qrx(min_id0);
            offy=qry(min_id0);
            for i=1:N
            qrx(i)=qrx(i)-x;
            qry(i)=qry(i)-y;
            end
            clf; 
            for i=1:N
            pos(1)=qrx(i);
            pos(2)=qry(i);
            pos(3)=0;
            pos(4)=qryaw(i);
            draw_mark(pos,MARK_L,2,'-k');
            str = [' ',num2str(id(i))];
            text(pos(1)+MARK_L/2,pos(2)+MARK_L/2,str,'Color','blue','FontSize',10);
            hold on;
            grid on;
            axis equal;
            title('Reset Confirm and Redrawed Marker ');
            end 
            ylim=get(gca,'Ylim'); % 获取当前图形的纵轴的范围
            hold on
            plot([0,0],ylim,'m--','LineWidth',2); % 绘制x=1的直线
            xlim=get(gca,'Xlim'); % 获取当前图形的纵轴的范围
            hold on
            plot(xlim,[0,0],'m--','LineWidth',2); % 绘制x=1的直线
            xlabel('Axis-X(m)')
            ylabel('Axis-Y(m)')
          end   
          if(mid==2&&select_reset==0&&select==0)
            select_reset=1;
            title('Reset Coordinate or Saving?');
          end 
          if(select_reset==2)
             select_reset=0; 
          end
        
 
          if(mid==2&&select==1)
            select_id=0;
            select=0;
             title('Clear Select');
             fprintf('Clear Select\n');
          end   
        
    end%end init
end

function draw_mark(pos,size,line_w,line_c)
%   4 1
%   3 2
yaw=pos(4);
x=pos(1);
y=pos(2);
r=sqrt(((size)/2)^2*2);
point=zeros(2,4);
point(1,1)=sind(45+yaw)*r+x;
point(2,1)=cosd(45+yaw)*r+y;
point(1,2)=sind(45+90*1+yaw)*r+x;
point(2,2)=cosd(45+90*1+yaw)*r+y;
point(1,3)=sind(45+90*2+yaw)*r+x;
point(2,3)=cosd(45+90*2+yaw)*r+y;
point(1,4)=sind(45+90*3+yaw)*r+x;
point(2,4)=cosd(45+90*3+yaw)*r+y;
%draw mark
plot([point(1,1), point(1,2)],[point(2,1), point(2,2)],line_c,'LineWidth',line_w);
hold on;
plot([point(1,2), point(1,3)],[point(2,2), point(2,3)],line_c,'LineWidth',line_w);
hold on;
plot([point(1,3), point(1,4)],[point(2,3), point(2,4)],line_c,'LineWidth',line_w);
hold on;
plot([point(1,4), point(1,1)],[point(2,4), point(2,1)],line_c,'LineWidth',line_w);
hold on;
%draw axis
x1=sind(0+yaw)*r*1.15+x;
y1=cosd(0+yaw)*r*1.15+y;
x2=sind(90+yaw)*r*1.15+x;
y2=cosd(90+yaw)*r*1.15+y;
plot([x,x1],[y,y1],'-r','LineWidth',line_w*1.5);
hold on;
plot([x,x2],[y,y2],'-b','LineWidth',line_w*1.5);
end

function [yaw]=To_180_degrees(x)
    if(x>180)
       yaw=x-360;
    else
       if(x<-180)
         yaw=x+360;
       else
         yaw=x;
       end  
    end    
end


