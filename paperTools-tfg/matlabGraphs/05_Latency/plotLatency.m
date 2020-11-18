%  Collect data 
mytable = readtable('05_Latency.csv');
values = mytable{:, :};
xdp_e_top = [0.0516 0.1871 0.2171];
xdp_e_bot = [0.0694 0.1059 0.2469];
p4_e_top = [0.1567 0.1588 0.3367];
p4_e_bot = [0.3563 0.5832 0.9133];

e_bot = [0.0694 0.1059 0.2469; 0.3563 0.5832 0.9133];
e_top = [0.0516 0.1871 0.2171; 0.1567 0.1588 0.2367];

% Plot
hFig = figure;
hB = bar(values,1,'grouped','FaceColor','flat');

%%For MATLAB R2019a or earlier releases
hold on

set(gca,'XTickLabel', {'XDP' 'P4'});
title('Latency');
grid on
ylim([0, 9]);
%xlabel('Kpps - 64B')
ylabel('Round Trip Time (ms)')

% IF we want to add Value Labels:
%
 for i=1:length(hB)  % iterate over number of bar objects
   text(hB(i).XData+hB(i).XOffset,hB(i).YData+0.33,string(hB(i).YData),'VerticalAlignment','bottom','horizontalalign','center');
 end

 
% Errors bars 
% Find the number of groups and the number of bars in each group
ngroups = size(values, 1);
nbars = size(values, 2);

% Calculate the width for each bar group
groupwidth = min(0.8, nbars/(nbars + 1.5));

% Set the position of each error bar in the centre of the main bar
% Based on barweb.m by Bolu Ajiboye from MATLAB File Exchange
for i = 1:nbars
    % Calculate center of each bar
    x = (1:ngroups) - groupwidth/2 + (2*i-1) * groupwidth / (2*nbars);
    errorbar(x, values(:,i), e_bot(:,i),e_top(:,i), 'k', 'linestyle', 'none');
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
