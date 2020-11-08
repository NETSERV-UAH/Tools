%  Collect data 
mytable = readtable('05_Latency.csv');
values = mytable{:, :};

% Plot
hFig = figure;
hB = bar(values,1,'grouped','FaceColor','flat');
set(gca,'XTickLabel', {'XDP' 'P4'});
title('Latency');
grid on
ylim([0, 9]);
%xlabel('Kpps - 64B')
ylabel('Round Trip Time (ms)')

% IF we want to add Value Labels:
%
 for i=1:length(hB)  % iterate over number of bar objects
   text(hB(i).XData+hB(i).XOffset,hB(i).YData,string(hB(i).YData),'VerticalAlignment','bottom','horizontalalign','center');
 end

%   Colors
 set(hB(1), 'FaceColor', [0.3010, 0.7450, 0.9330]);
 set(hB(2), 'FaceColor', rgb(Color,'gold'));
 set(hB(3), 'FaceColor', rgb(Color,'brown'));

% set(hB(1), 'FaceColor', 'g');

legend(hB,'min','avg','max', 'Location', 'NorthWest', 'Orientation', 'vertical');

% Export figure
set(hFig,'Units','Inches');
pos = get(hFig,'Position');
set(hFig,'PaperPositionMode','Auto','PaperUnits','Inches','PaperSize',[pos(3), pos(4)])
print(hFig,'05_Latency','-dpdf','-r0')
