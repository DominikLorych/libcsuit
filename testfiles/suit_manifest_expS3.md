<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 3: Integrated Dependency Example
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope_Tagged / 107({
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'318EAD5F671A6D2593D7ADB7B6CCADC49F72704507004F297A25AF16A48A2111'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'287D5AAB44D08A34954663942B2732825426893ACD735BF3A79B8B5B38EC3C9950D917D72D5586867C8FF58CF5827B0C2B94952359C3971DBF202B0774627DC3'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 0,
    / common / 3: << {
      / dependencies / 1: {
        / component-index / 1: {
          / dependency-prefix / 1: [
            'dependent.suit'
          ]
        }
      },
      / components / 2: [
        [
          '10'
        ]
      ]
    } >>,
    / manifest-component-id / 5: [
      'depending.suit'
    ],
    / invoke / 9: << [
      / directive-set-component-index / 12, 0,
      / directive-override-parameters / 20, {
        / parameter-invoke-args / 23: 'cat 00 10'
      },
      / directive-invoke / 23, 15
    ] >>,
    / dependency-resolution / 15: << [
      / directive-set-component-index / 12, 1,
      / directive-override-parameters / 20, {
        / parameter-image-digest / 3: << [
          / digest-algorithm-id: / -16 / SHA256 /,
          / digest-bytes: / h'6c86246b90d644f021671f6d42523b2cb5e156f764be618aa46bfcd0db23e768'
        ] >>,
        / parameter-image-size / 14: 352,
        / parameter-uri / 21: "#dependent.suit"
      },
      / directive-fetch / 21, 2,
      / condition-image-match / 3, 15
    ] >>,
    / install / 17: << [
      / directive-set-component-index / 12, 1,
      / directive-process-dependency / 11, 0,

      / directive-set-component-index / 12, 0,
      / directive-override-parameters / 20, {
        / parameter-content / 18: ' in multiple trust domains'
      },
      / directive-write / 18, 15
    ] >>
  } >>,
  "#dependent.suit": h'D86BA301589E8181589AD28443A10126A0584FA108A101A4010220012158200E908AA8F066DB1F084E0C3652C63952BD99F2A5BDB22F9E01367AAD03ABA68B22582077DA1BD8AC4F0CB490BA210648BF79AB164D49AD3551D71D314B2749EE42D29A5840FB2D5ACF66B9C8573CE92E13BFB8D113F798715CC10B5A0010B11925C155E7245A64E131073B87AC50CAC71650A21315B82D06CA2298CD1A95519AAE4C4B5315025874835824822F58206EA128D7BB19B86F77C4227F2A29F22026A41958ACC45CC0A35BA388B13E2F51584AD28443A10126A0F6584099F949043701D7BDBA38904A0B49F004DED6B64A4900DECA5C66AE8A9EBA913576DEF136B74EA89C14FA64624DBD33B4C0BB41C153CA51548C73FF71A2BAF27440035842A6010102000347A102818142303005814E646570656E64656E742E73756974094D8414A11746636174203030170F11528414A1124B68656C6C6F20776F726C64120F'
})
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
D86BA3025873825824822F5820318EAD5F671A6D2593D7ADB7B6CCADC49F
72704507004F297A25AF16A48A2111584AD28443A10126A0F65840287D5A
AB44D08A34954663942B2732825426893ACD735BF3A79B8B5B38EC3C9950
D917D72D5586867C8FF58CF5827B0C2B94952359C3971DBF202B0774627D
C30358BCA70101020003581CA201A101A101814E646570656E64656E742E
7375697402818142313005814E646570656E64696E672E73756974095286
0C0014A11749636174203030203130170F0F5845880C0114A3035824822F
58206C86246B90D644F021671F6D42523B2CB5E156F764BE618AA46BFCD0
DB23E7680E190160156F23646570656E64656E742E737569741502030F11
58288A0C010B000C0014A112581A20696E206D756C7469706C6520747275
737420646F6D61696E73120F6F23646570656E64656E742E737569745901
60D86BA301589E8181589AD28443A10126A0584FA108A101A40102200121
58200E908AA8F066DB1F084E0C3652C63952BD99F2A5BDB22F9E01367AAD
03ABA68B22582077DA1BD8AC4F0CB490BA210648BF79AB164D49AD3551D7
1D314B2749EE42D29A5840FB2D5ACF66B9C8573CE92E13BFB8D113F79871
5CC10B5A0010B11925C155E7245A64E131073B87AC50CAC71650A21315B8
2D06CA2298CD1A95519AAE4C4B5315025874835824822F58206EA128D7BB
19B86F77C4227F2A29F22026A41958ACC45CC0A35BA388B13E2F51584AD2
8443A10126A0F6584099F949043701D7BDBA38904A0B49F004DED6B64A49
00DECA5C66AE8A9EBA913576DEF136B74EA89C14FA64624DBD33B4C0BB41
C153CA51548C73FF71A2BAF27440035842A6010102000347A10281814230
3005814E646570656E64656E742E73756974094D8414A117466361742030
30170F11528414A1124B68656C6C6F20776F726C64120F
~~~~
