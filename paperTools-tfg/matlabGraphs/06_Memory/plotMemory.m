%  Collect data 
mytable = readtable('06_Memory_withoutSharedObjects.csv');
values = mytable{:, :};


% Plot
hFig = figure;
aHand = axes('parent', hFig);
hold(aHand, 'on')
colors = hsv(numel(values));
hB = bar(1, values(1), 'parent', aHand, 'facecolor', [0, 0.4470, 0.7410]);
hBr = bar(2, values(2), 'parent', aHand, 'facecolor', [0.8500, 0.3250, 0.0980]);
title('Memory consumed');
grid on
ylim([0, 10000]);
%xlabel('Kpps - 64B')
ylabel('Memory (KB)')
set(gca, 'XTick', 1:numel(values), 'XTickLabel', {'XDP', 'P4'})

% IF we want to add Value Labels:
%
 for i=1:length(hB)  % iterate over number of bar objects
   text(hB(i).XData+hB(i).XOffset,hB(i).YData,string(hB(i).YData),'VerticalAlignment','bottom','horizontalalign','center');
 end
 for i=1:length(hBr)  % iterate over number of bar objects
   text(hBr(i).XData+hBr(i).XOffset,hBr(i).YData,string(hBr(i).YData),'VerticalAlignment','bottom','horizontalalign','center');
 end
%   Colors
%  set(hB(1), 'FaceColor', [0.3010, 0.7450, 0.9330]);
%  set(hB(2), 'FaceColor', rgb(Color,'gold'));
%  set(hB(3), 'FaceColor', rgb(Color,'brown'));

% set(hB(1), 'FaceColor', 'g');
%legend(hB,'min','avg','max', 'Location', 'NorthWest', 'Orientation', 'vertical');


% Export figure
set(hFig,'Units','Inches');
pos = get(hFig,'Position');
set(hFig,'PaperPositionMode','Auto','PaperUnits','Inches','PaperSize',[pos(3), pos(4)])
print(hFig,'06_Memory_withoutSharedObjects','-dpdf','-r0')

