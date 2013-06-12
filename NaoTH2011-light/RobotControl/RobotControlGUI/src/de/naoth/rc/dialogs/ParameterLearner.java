/*
 * ParameterLearner.java
 *
 * Created on Jun 7, 2013, 4:32:39 PM
 */
package de.naoth.rc.dialogs;

import de.naoth.rc.AbstractDialog;
import de.naoth.rc.RobotControl;
import de.naoth.rc.server.Command;
import de.naoth.rc.server.CommandSender;
import java.awt.event.KeyEvent;
import java.util.HashMap;
import java.util.Map;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import net.xeoh.plugins.base.annotations.PluginImplementation;
import net.xeoh.plugins.base.annotations.events.Init;
import net.xeoh.plugins.base.annotations.injections.InjectPlugin;

/**
 *
 * @author auke
 */
@PluginImplementation
public class ParameterLearner extends AbstractDialog implements CommandSender {

  private Command commandToExecute;
  private final String strIKParameters = "ParameterList:IKParameters";
  private final String strMLParameters = "ParameterList:MachineLearningParamters";
  
  @InjectPlugin
  public RobotControl parent;

  Map<String, String> commandMap = new HashMap<String, String>();

    /** Creates new form ParameterLearner */
     public ParameterLearner() {
        initComponents();
    }

  @Init
  public void init()
  {
    if(parent == null)
    {
      throw (new IllegalArgumentException("\"parent\" was null"));
    }
  }

  @Override
  public JPanel getPanel()
  {
    return this;
  }

private static Command parseTextArea(String cmdName, String text)
{
    Command cmd = new Command(cmdName);
    
    text = text.replaceAll("( |\t)+", "");
    String[] lines = text.split("(\n)+");
    for (String l : lines)
    {
      String[] splitted = l.split("=");
      if (splitted.length == 2)
      {
        String key = splitted[0].trim();
        String value = splitted[1].trim();
        // remove the last ;
        if (value.charAt(value.length() - 1) == ';')
        {
          value = value.substring(0, value.length() - 1);
        }

        cmd.addArg(key, value);
      }
    } // end for
    return cmd;
} // end parseTextArea
  
private void saveWalkingParameters() {
  if (parent.checkConnected())
  {
    getWalkingParameterList();
    Command cmd = parseTextArea(strIKParameters + ":set", 
                                this.jTextAreaWalkingParams.getText());
    sendCommand(cmd);
    // stop learning when walking parameters are saved , for now
    // TODO save somewhere externally, or under some specific name
    jToggleButtonLearn.setSelected(false);
  }
  else
  {
    jToggleButtonLearn.setSelected(false);
  }    
}

private boolean getParameterList(String strCommand)
{
    if (parent.checkConnected())
    {
      Command cmd = new Command(strCommand);
      sendCommand(cmd);
      return true;
    }
    return false;
}

private void getWalkingParameterList() {
    if(!getParameterList(strIKParameters + ":get"))
    {
         jToggleButtonReceive.setSelected(false);
    }
}//end getWalkingParameterList

private void getLearningParameterList() {
    if(!getParameterList(strMLParameters + ":get"))
    {
        jToggleButtonLearn.setSelected(false);
    }
}//end getLearningParameterList

private void sendLearningParameters()
{
  if (parent.checkConnected())
  {
     // TODO add combobox method name, send correct parameters only
    Command cmd = parseTextArea(strMLParameters + ":set",
            this.jTextAreaLearningParams.getText());
    //Command cmd = new Command("ParameterList:" + cbLearningMethod.getSelectedItem().toString() + ":set");

    sendCommand(cmd);
    getLearningParameterList();
  }
  else
  {
    jToggleButtonLearn.setSelected(false);
  }
}

private void sendCommand(Command command)
  {
    commandToExecute = command;
    parent.getMessageServer().executeSingleCommand(this, command);
  }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jScrollPane3 = new javax.swing.JScrollPane();
        jTable1 = new javax.swing.JTable();
        jScrollPane1 = new javax.swing.JScrollPane();
        jTextAreaWalkingParams = new javax.swing.JTextArea();
        jScrollPane2 = new javax.swing.JScrollPane();
        jTextAreaLearningParams = new javax.swing.JTextArea();
        filler1 = new javax.swing.Box.Filler(new java.awt.Dimension(0, 0), new java.awt.Dimension(0, 0), new java.awt.Dimension(0, 0));
        jTextFieldConsole = new javax.swing.JTextField();
        jLabelInfoDisplay = new javax.swing.JLabel();
        jToolBar1 = new javax.swing.JToolBar();
        jToggleButtonReceive = new javax.swing.JToggleButton();
        jButtonSave = new javax.swing.JButton();
        jToolBar2 = new javax.swing.JToolBar();
        jToggleButtonLearn = new javax.swing.JToggleButton();
        jButtonSetLP = new javax.swing.JButton();
        jButtonGetLP = new javax.swing.JButton();
        cbLearningMethod = new javax.swing.JComboBox();

        jTable1.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {
                {null, null, null, null},
                {null, null, null, null},
                {null, null, null, null},
                {null, null, null, null}
            },
            new String [] {
                "Title 1", "Title 2", "Title 3", "Title 4"
            }
        ));
        jScrollPane3.setViewportView(jTable1);

        jTextAreaWalkingParams.setColumns(20);
        jTextAreaWalkingParams.setRows(5);
        jScrollPane1.setViewportView(jTextAreaWalkingParams);

        jTextAreaLearningParams.setColumns(20);
        jTextAreaLearningParams.setRows(5);
        jScrollPane2.setViewportView(jTextAreaLearningParams);

        jTextFieldConsole.setText("Console");
        jTextFieldConsole.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                jTextFieldConsoleKeyReleased(evt);
            }
        });

        jLabelInfoDisplay.setText("Infodisplay");

        jToolBar1.setRollover(true);

        jToggleButtonReceive.setText("Receive");
        jToggleButtonReceive.setFocusable(false);
        jToggleButtonReceive.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jToggleButtonReceive.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jToggleButtonReceive.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jToggleButtonReceiveMouseClicked(evt);
            }
        });
        jToolBar1.add(jToggleButtonReceive);

        jButtonSave.setText("Save");
        jButtonSave.setFocusable(false);
        jButtonSave.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jButtonSave.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jButtonSave.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButtonSaveMouseClicked(evt);
            }
        });
        jToolBar1.add(jButtonSave);

        jToolBar2.setRollover(true);

        jToggleButtonLearn.setText("Learn");
        jToggleButtonLearn.setFocusable(false);
        jToggleButtonLearn.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jToggleButtonLearn.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jToggleButtonLearn.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jToggleButtonLearnActionPerformed(evt);
            }
        });
        jToolBar2.add(jToggleButtonLearn);

        jButtonSetLP.setText("Set");
        jButtonSetLP.setFocusable(false);
        jButtonSetLP.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jButtonSetLP.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jButtonSetLP.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButtonSetLPMouseClicked(evt);
            }
        });
        jToolBar2.add(jButtonSetLP);

        jButtonGetLP.setText("Get");
        jButtonGetLP.setFocusable(false);
        jButtonGetLP.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jButtonGetLP.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jButtonGetLP.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButtonGetLPMouseClicked(evt);
            }
        });
        jToolBar2.add(jButtonGetLP);

        cbLearningMethod.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "Item 1", "Item 2", "Item 3", "Item 4" }));
        cbLearningMethod.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cbLearningMethodActionPerformed(evt);
            }
        });
        jToolBar2.add(cbLearningMethod);

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jTextFieldConsole)
            .add(layout.createSequentialGroup()
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING)
                    .add(jToolBar2, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 250, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                    .add(jScrollPane2, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 250, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(layout.createSequentialGroup()
                        .add(28, 28, 28)
                        .add(filler1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                    .add(layout.createSequentialGroup()
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(jScrollPane1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 250, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                            .add(jToolBar1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 250, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))))
                .add(0, 0, Short.MAX_VALUE))
            .add(jLabelInfoDisplay, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, layout.createSequentialGroup()
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(jToolBar1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 25, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                    .add(jToolBar2, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 25, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING)
                    .add(jScrollPane1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 152, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                    .add(jScrollPane2, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 152, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jTextFieldConsole, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .add(18, 18, 18)
                .add(filler1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jLabelInfoDisplay, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 152, Short.MAX_VALUE)
                .addContainerGap())
        );
    }// </editor-fold>//GEN-END:initComponents

    private void jTextFieldConsoleKeyReleased(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_jTextFieldConsoleKeyReleased
        int key = evt.getKeyCode();
        if (key == KeyEvent.VK_ENTER)
        {
            Command command = new Command(this.jTextFieldConsole.getText());
            sendCommand(command);
            int k = jTextFieldConsole.getCaretPosition();
            if(k > 0)
               jTextFieldConsole.setCaretPosition(k-1);
        }
    }//GEN-LAST:event_jTextFieldConsoleKeyReleased

    private void jButtonSetLPMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButtonSetLPMouseClicked
        sendLearningParameters();
    }//GEN-LAST:event_jButtonSetLPMouseClicked

    private void jButtonSaveMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButtonSaveMouseClicked
        saveWalkingParameters();
    }//GEN-LAST:event_jButtonSaveMouseClicked

    private void jToggleButtonReceiveMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jToggleButtonReceiveMouseClicked
        // TODO Keep receiving walking params
    }//GEN-LAST:event_jToggleButtonReceiveMouseClicked

    private void jButtonGetLPMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButtonGetLPMouseClicked
        getLearningParameterList();
    }//GEN-LAST:event_jButtonGetLPMouseClicked

    private void jToggleButtonLearnActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jToggleButtonLearnActionPerformed
    if (jToggleButtonLearn.isSelected())
        {
        if (parent.checkConnected()) {
         // TODO create manager which refreshes commands, see debugrequestmanager
          } else {
            jToggleButtonLearn.setSelected(false);
          }
        } else {
          // TODO remove listener
        }
    }//GEN-LAST:event_jToggleButtonLearnActionPerformed

    private void cbLearningMethodActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_cbLearningMethodActionPerformed
        getLearningParameterList();
    }//GEN-LAST:event_cbLearningMethodActionPerformed

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(ParameterLearner.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(ParameterLearner.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(ParameterLearner.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(ParameterLearner.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {

            @Override
            public void run() {
                new ParameterLearner().setVisible(true);
            }
        });
    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JComboBox cbLearningMethod;
    private javax.swing.Box.Filler filler1;
    private javax.swing.JButton jButtonGetLP;
    private javax.swing.JButton jButtonSave;
    private javax.swing.JButton jButtonSetLP;
    private javax.swing.JLabel jLabelInfoDisplay;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JScrollPane jScrollPane3;
    private javax.swing.JTable jTable1;
    private javax.swing.JTextArea jTextAreaLearningParams;
    private javax.swing.JTextArea jTextAreaWalkingParams;
    private javax.swing.JTextField jTextFieldConsole;
    private javax.swing.JToggleButton jToggleButtonLearn;
    private javax.swing.JToggleButton jToggleButtonReceive;
    private javax.swing.JToolBar jToolBar1;
    private javax.swing.JToolBar jToolBar2;
    // End of variables declaration//GEN-END:variables

  @Override
  public void handleResponse(byte[] result, Command originalCommand)
  {
    String strResult = new String(result);
    if (strResult.contains("[DebugServer] unknown command: "))
    {
      JOptionPane.showMessageDialog(this, "Can not send parameters!", 
              "Parameter Learner", JOptionPane.ERROR_MESSAGE);
      jToggleButtonLearn.setSelected(false);
    }
    else
    {
        String strCommand = originalCommand.getName();
        if(strCommand.compareTo(strIKParameters + ":get") == 0)
        {
            jTextAreaWalkingParams.setText(strResult);
        } 
        else if(strCommand.compareTo(strMLParameters + ":get") == 0)
        {
            String selectedMethod = null;
            if (cbLearningMethod.getSelectedItem() != null)
                selectedMethod = cbLearningMethod.getSelectedItem().toString();
            
            cbLearningMethod.removeAllItems();
            
            String[] mlParameterList = strResult.split("\n");
            // TODO iterate over parameterList, find out which methods
            // exist based on prefix (separate by '.'), list them in combobox
            
            for (String method : mlParameterList)
            {
              cbLearningMethod.addItem(method);
            }

            // try to set back the selection
            if(selectedMethod != null)
                cbLearningMethod.setSelectedItem(selectedMethod);

            jToggleButtonLearn.setSelected(false);
        } 
    }
  }//end handleResponse

  @Override
  public void handleError(int code)
  {
    jToggleButtonLearn.setSelected(false);
    JOptionPane.showMessageDialog(this,
              "Error occured, code " + code, "ERROR", JOptionPane.ERROR_MESSAGE);
  }//end handleError

  @Override
  public Command getCurrentCommand()
  {
    return commandToExecute;
  }
}