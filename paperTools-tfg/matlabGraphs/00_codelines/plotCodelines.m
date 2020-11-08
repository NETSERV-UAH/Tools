%  Collect data 
mytable = readtable('00_codelines.csv');
values = mytable{:, :};

% Plot
hFig = figure;
hB = bar(values,'grouped','FaceColor','flat');
set(gca,'XTickLabel', {'XDP wired' 'XDP wireless' 'P4 wired' 'P4 wireless' });
title('Number of code lines');
grid on
ylim([0, 320]);

% IF we want to add Value Labels:
%
% for i=1:length(hB)  % iterate over number of bar objects
%   text(hB(i).XData+hB(i).XOffset,hB(i).YData,string(hB(i).YData),'VerticalAlignment','bottom','horizontalalign','center');
% end

%   Colors
% set(hB(1), 'FaceColor', [1 0 0]);
% set(hB(1), 'FaceColor', 'g');

legend(hB,'case01','case02','case03','case04', 'case05', 'Location', 'SouthOutside', 'Orientation', 'horizontal');

% Export figure
set(hFig,'Units','Inches');
pos = get(hFig,'Position');
set(hFig,'PaperPositionMode','Auto','PaperUnits','Inches','PaperSize',[pos(3), pos(4)])
print(hFig,'00_codelines','-dpdf','-r0')
