program prjServer;

uses
  System.StartUpCopy,
  FMX.Forms,
  uServer in 'uServer.pas' {FrmMain};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TFrmMain, FrmMain);
  Application.Run;
end.
