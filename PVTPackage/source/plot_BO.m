function plot_BO()
close all;

M = dlmread('E:\GithubRepository\PVTPackage\build\examples\BlackOil\PVTO.txt');
Rs = M(:,1);
Pbub = M(:,2);
Bo = M(:,3);
visc = M(:,4);


hold on

yyaxis left
plot(Pbub,Bo,'+');
yyaxis right
plot(Pbub,visc,'o');
hold off

% figure();
% 
% M = dlmread('E:\GithubRepository\PVTPackage\build\examples\BlackOil\PVTG.txt');
% Pdew = M(:,1);
% Rv = M(:,2);
% Bg = M(:,3);
% visc = M(:,4);
% 
% 
% hold on
% 
% yyaxis left
% plot(Rv,Bg,'+');
% yyaxis right
% % plot(Rv,visc,'o');
% hold off



end