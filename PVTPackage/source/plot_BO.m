function plot_BO()
close all;

Mo = dlmread('E:\GithubRepository\PVTPackage\build\examples\BlackOil\PVTO.txt');
Mg = dlmread('E:\GithubRepository\PVTPackage\build\examples\BlackOil\PVTG.txt');

Rs = Mo(:,1);
Pbub = Mo(:,2);
Bo = Mo(:,3);
visco_o = Mo(:,4);

Pdew = Mg(:,1);
Rv = Mg(:,2);
Bg = Mg(:,3);
visco_g = Mg(:,4);

hold on
yyaxis left
plot(Pbub,Bo,'+');
yyaxis right
plot(Pbub,Rs,'o');
hold off

figure()
plot(Pbub(1:100:end),Rs(1:100:end),'o');


% figure();
% 
% hold on
% yyaxis left
% plot(Pdew,Bg,'+');
% yyaxis right
% plot(Pdew,visco_g,'o');
% hold off


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