%  Collect data 
mytable = readtable('04_CPU.csv');
values = transpose(mytable{:, :});

% Plot
hFig = figure;
hB = bar(values,'grouped','FaceColor','flat');
set(gca,'XTickLabel', {'0.5' '1.5' '4.5' '13.5' '40' '100' '150'});
title('CPU usage');
grid on
ylim([0, 100]);
xlabel('Kpps - 64B')
ylabel('%CPU')
% IF we want to add Value Labels:
%
% for i=1:length(hB)  % iterate over number of bar objects
%   text(hB(i).XData+hB(i).XOffset,hB(i).YData,string(hB(i).YData),'VerticalAlignment','bottom','horizontalalign','center');
% end

%   Colors
% set(hB(1), 'FaceColor', [1 0 0]);
% set(hB(1), 'FaceColor', 'g');

legend(hB,'XDP','P4', 'Location', 'NorthWest', 'Orientation', 'vertical');

% Export figure
set(hFig,'Units','Inches');
pos = get(hFig,'Position');
set(hFig,'PaperPositionMode','Auto','PaperUnits','Inches','PaperSize',[pos(3), pos(4)])
print(hFig,'00_CPU','-dpdf','-r0')
