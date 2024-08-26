@page fdas_pipeline_guide FDAS pipeline guide
# Notes for Developers
## FDAS pipeline
This pipeline will perform a Fourier-domain acceleration search on input DmTime data (DmTime data consist of lists of DmTrials, which themselves are a data type that stores the amplitude of a signal versus time, dedispersed at some dispersion measure; see cheetah/data/DmTime.h for more details). This is generally comprised of 4 main steps:
- Fourier transform
- Acceleration search
- Harmonic summing
- Thresholding

### Fourier transform
The input data (in DmTime format, so time-domain data) are transformed into frequency-domain data, which will make periodic (in time) signals more detectable. This is generally done with a Fast Fourier Transform (FFT), but can differ for a given computing architecture. Regardless, the Fourier transform is a set mathematical procedure.

### Acceleration search
Normal emission from a stationary pulsar will appear in one or two bins in the Fourier spectrum, depending on the exact frequency of the pulsar and the size of the bins in the spectrum (and ignoring harmonics, which are covered next). However, if a pulsar is in orbit around another object, then the pulsar will likely be moving toward and away from an observer at different points in its orbit. This motion relative to the observer will cause a doppler shift in the received frequency of the pulsar emission, so the power in the spectrum will spread over a number of neighbouring bins. The shape of this spread is well defined by a mathematical function, so templates can be created to use in matched filtering.

### Harmonic summing
Harmonically-related bins in the Fourier spectrum are then summed. This is required because the power in the signal from a pulsar is generally split into multiple harmonics over many spectral bins. This is due to the fact that the duty cycle of pulsar emission tends to be low, meaning that there are a large number of sinusoids required to reproduce such a narrow, repeated pulse. The number of harmonics over which a signal from a particular pulsar are spread is dependent on the exact duty cycle of that pulsar, so we need to sum up many different harmonics in order to maximise the signal power from the pulsar without adding undue noise. This summing can be accomplished by stretching the spectrum a certain amount, depending on the particular harmonic being summed, and adding it "on top of" the original spectrum. Note, this decreases the frequency resolution of the summed spectrum, as the frequency bins in the stretched spectrum are themselves stretched.

### Thresholding
Once a number of harmonically-summed spectra have been produced, statistically significant peaks are found by looking for all signals above a certain S/N threshold. The frequencies of these peaks are recorded for subsequent folding. Note that thresholding can be carried out at multiple stages, e.g. on the Fourier spectrum after matched filtering, after summing two harmonics, after summing four harmonics, etc.
