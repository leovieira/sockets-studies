program prjClient;

uses
  System.StartUpCopy,
  FMX.Forms,
  uClient in 'uClient.pas' {FrmMain};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TFrmMain, FrmMain);
  Application.Run;
end.
