unit uServer;

interface

uses
  System.SysUtils, System.Types, System.UITypes, System.Classes, System.Variants,
  FMX.Types, FMX.Controls, FMX.Forms, FMX.Graphics, FMX.Dialogs, FMX.ScrollBox,
  FMX.Memo, FMX.StdCtrls, FMX.Edit, FMX.Controls.Presentation, IdBaseComponent,
  IdComponent, IdCustomTCPServer, IdTCPServer, FMX.Objects, IdContext,
  System.JSON;

type
  TFrmMain = class(TForm)
    GroupBox1: TGroupBox;
    Label1: TLabel;
    Edit1: TEdit;
    Label2: TLabel;
    Switch1: TSwitch;
    Memo1: TMemo;
    Rectangle1: TRectangle;
    Circle1: TCircle;
    StyleBook1: TStyleBook;
    IdTCPServer1: TIdTCPServer;
    procedure Switch1Switch(Sender: TObject);
    procedure IdTCPServer1Execute(AContext: TIdContext);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  FrmMain: TFrmMain;

implementation

{$R *.fmx}

procedure TFrmMain.Switch1Switch(Sender: TObject);
begin
  try
    if Switch1.IsChecked then
    begin
      IdTCPServer1.DefaultPort := Edit1.Text.ToInteger;
      IdTCPServer1.Active := true;
      Memo1.Lines.Add('Server started!');
    end
    else
    begin
      IdTCPServer1.Active := false;
      Memo1.Lines.Add('Server finished!');
    end;
  except
    IdTCPServer1.Active := false;
    Memo1.Lines.Add('Oops ... something went wrong!');
  end;
end;

procedure TFrmMain.IdTCPServer1Execute(AContext: TIdContext);
var
  result, position: TJSONObject;
  x, y: integer;
begin
  result := TJSONObject.ParseJSONValue(AContext.Connection.Socket.ReadLn) as TJSONObject;
  position := result.GetValue<TJSONObject>('position') as TJSONObject;

  x := position.GetValue<integer>('x');
  y := position.GetValue<integer>('y');

  Circle1.Position.X := x;
  Circle1.Position.Y := y;
end;

end.

