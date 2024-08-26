# SKA PSS Input parameters 

The PSS DDD and the CSP to Sub-elements ICD define a set of parameters to
configure PSS working durin scans. The current list is up to date on 21 Mar
2018.

## General parameters

| CSP_PSS Parameter    | CTRL Name       | Type         | Range     |  Modules | 
| -------------------- | --------------- | ------------ | --------- | -------- | 
| Sub-array ID         | sub_array_id    | ASCII String | 0-15      |  All     |
| Action               | action          | ASCII String | Set       |  All     |
| Activation Time      | activation_time | Date & time  | UTC       |  All     |
| Duration             | duration        | Integer      | 0-2100s   |  All     |
| Scan ID              | scan_id         | Integer      | 64-bit    |  All     |
| Observing Mode       | observing_mode  | ASCII String | Pulsar and/or Single pulse| All  |
| Pointing Name        | pointing_name   | ASCII String | String    |  All     |
| Pointing coordinates | pointing_coord  | ASCII String | Astronomical Coordinates  | CDOS |

## Parameters specific for each sub-array

<table>
<tr><th>CSP_PSS Parameter         <th>CTRL Name        <th>Type     <th>Range                     <th> Modules 
<tr><td>Beam BW                   <td>beam_bw          <td>Double   <td> 96 MHz LOW<br>300MHz MID <td> RCPT<br>FLDO
<tr><td>Number of bits per sample <td>bit_per_sample   <td>Integer  <td> 1-32                     <td> RCPT<br>FLDO
<tr><td>Acceleration Search       <td>accel_search     <td>Boolean  <td> Enable / Disable         <td> CTRL<br>Cheetah
<tr><td>Single Pulse Search       <td>single_p_search  <td>Boolean  <td> Enable / Disable         <td> CTRL<br>Cheetah
<tr><td>Integration time          <td>integration_time <td>Double   <td> Up to 1,800 seconds      <td> RCPT<br>FLDO<br>Cheetah
<tr><td>Acceleration Range        <td>accel_range      <td>Double   <td> +/-350 m/s^2  Default=0  <td> FDAS<br>TDAS
<tr><td>Number of Trials          <td>trials_number    <td>Integer  <td> 0-Maxint (band dependent)<td> FDAS<br>TDAS
<tr><td>Time Resolution           <td>time_resolution  <td>Double   <td> 50-800us 22n * 5us)      <td> RCPT<br>FLDO
<tr><td>Dispersion Measure        <td>disp_measure     <td>Double   <td> 0-3000 pc cm-3           <td> DRED<br>FDAS<br>TDAS<br>DTR
<tr><td>SPS_Dispersion Measure    <td>sps_disp_measure <td>Double   <td> 0-3000 pc cm-3           <td> DRED<br>SIFT<br>DTR
<tr><td>Number of Frequency Channels<td>freq_channels  <td>Integer  <td> 1000-8192                <td> All
<tr><td>Time Samples per Block    <td>num_samples      <td>Unsignd Long<td> Up to (integration time) / (time resolution) <td> All
<tr><td>Sub-bands                 <td>sub_bands        <td>Integer  <td> Up to 64                 <td> FLDO
<tr><td>Harmonic summing control parameter<td>harmonic_folds<td>Integer<td> 1-32                  <td> HRMS
<tr><td>Complex FFT Control Parameters  <td>cfft_control <td>String <td> Label or Structure       <td> CXFTC
<tr><td>Candidates Output Parameters <td>candidate_sifting<td>String<td> Label or Structure       <td> SIFT
<tr><td>DRED Control Parameters   <td>dred_statistic   <td>String   <td> Label or Structure       <td> DRED
<tr><td>CDOS Control Parameters   <td>cdos_control     <td>String   <td> Label or Structure       <td> CDOS
<tr><td>FLDO Control Parameters   <td>cdos_control     <td>String   <td> Label or Structure       <td> FLDO
<tr><td>RFIM Control Parameters   <td>cdos_control     <td>String   <td> Label or Structure       <td> RFIM
<tr><td>Single Pulse Threshold    <td>single_threshold <td>Double   <td> Tuned to system noise    <td> SIFT<br>SPD 
</table>

NOTE: the string variables labeled as _Label or Structure_ can contain or
a  pointer to an external file (_Label_ a symbolic name or a file-path)  or a 
*json* _Structure_ containing low level engineering parameters.


## Parameters specific for each beam

<table>
<tr><th>CSP_PSS Parameter         <th>CTRL Name        <th>Type     <th>Range                     <th> Modules 
<tr><td>Beam ID                   <td>beam_id          <td>Integer  <td> 0-Maxint                 <td> All
<tr><td>Destination address       <td>dest_address     <td>String   <td> IP address and port      <td> CDOS
<tr><td>Checksum                  <td>checksum         <td>Long     <td> 0-MaxLong                <td> CTRL



