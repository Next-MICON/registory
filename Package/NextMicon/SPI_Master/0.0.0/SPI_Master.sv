//////////////////////////////////////////////////////////////
// SPI_Master ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//       : _  :_   _   _   _   _   _   _  :_   _   _   _    //
// clk   :  |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_  //
//       :    :___:___:___________________:___:__           //
// wstrb : ___/   :   :                   :   :  \_________ //
//       :    :   :   :                   :   :   :         //
// state : w  X sending                   X end   X waiting //
//       :    :___:___:_______     _______:___:___:         //
// cnt   : ___/___0___X___1___X...X___7___X_0_X_1_\_0______ //
//       :    :   :___:    ___         ___:   :   :         //
// sclk  : _______/   \___/   \...\___/   \________________ //
//       :    :___:___:_______     _______:   :   :         //
// mosi  : ---<__[0]__X_[1]___X...X_[7]___>---------------- //
//       :    :___:___:_______     _______:   :   :         //
// miso  : ---<__[0]__X_[1]___X...X_[7]___>---------------- //
//       :    :   :   :                   :______ :         //
// ready : _______________________________/      \______    //
//       :    :   :   :                   :       :         //
//      [I]  [S] [A] [B]                 [E]     [W]        //
//////////////////////////////////////////////////////////////

// [S] 送受信はレジスタへの書き込み信号 wstrb によって開始されます
//     最初のデータをMOSIに出力します
// [A] ここでデータをキャプチャします
// [B] 次のデータをMOSIに出力します
// [E] 1 バイトの送受信を終了します
//     レジスタに受信データをセットし ready をアサートします
// ※ SSはGPIOで制御します

module SPI_Master #(
    parameter CPOL = 0,
    parameter CPHA = 0
) (
    input wire clk,
    input wire resetn,

    input wire valid,
    output reg ready,
    input wire [31:0] addr,
    input wire [3:0] wstrb,
    input wire [31:0] wdata,
    output reg [31:0] rdata,

    output reg  scl,
    output reg  mosi,
    input  wire miso
);

  reg [2:0] state;
  reg [2:0] cnt;
  reg [7:0] dsnd;
  reg [7:0] drcv;

  localparam waiting = 3'b001;
  localparam sending = 3'b010;
  localparam ending = 3'b100;

  always @(posedge clk) begin
    if (!resetn) begin  // [I] Initial state
      state <= waiting;
      cnt   <= 0;
      scl   <= 0;
      mosi  <= 0;
      dsnd  <= 0;
      drcv  <= 0;
    end else begin
      ready <= valid;
      rdata <= {16'b0, dsend};
      case (state)
        waiting: begin
          if (wstrb) begin  // [S] Start sending
            // Latch send data
            dsnd  <= wdata[7:0];
            // Start sending
            state <= sending;
            cnt   <= 0;
            scl   <= 0;
            mosi  <= wdata[7];
          end
        end
        sending: begin
          case (scl)
            0: begin  // [A]
              scl <= 1;
            end
            1: begin
              if (cnt == 15) begin  // [E] ending
                state <= ending;
                cs <= 1;
                cnt <= 0;
              end else begin  // [B]
                scl <= 0;
                cnt <= cnt + 1;
                sdi <= dsend[14-cnt];
              end
            end
          endcase
        end
        ending: begin
          case (cnt)
            0: begin
              cnt  <= cnt + 1;
              ldac <= 0;
            end
            1: begin
              cnt <= cnt + 1;
            end
            2: begin
              state <= waiting;
              ldac  <= 1;
            end
          endcase
        end
      endcase
    end
  end
endmodule
