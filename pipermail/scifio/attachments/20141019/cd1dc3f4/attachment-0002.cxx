#include "itkSCIFIOImageIO.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkRGBPixel.h"
#include "itkMetaDataObject.h"
#include "itkStreamingImageFilter.h"


int RunTest ( const char * inputFileName, const char * outputFileName, int seriesStart, int seriesEnd, int numberOfStreamDivisions, bool writeSCIFIO, bool allSeries )
{
  typedef itk::Image<unsigned short, 5> ImageType5D;
  typedef itk::Image<unsigned short, 4 > ImageType4D;
  typedef itk::Image<unsigned short, 3 > ImageType3D;
  typedef typename itk::ImageFileReader< ImageType5D >    ReaderType;

  itk::SCIFIOImageIO::Pointer io = itk::SCIFIOImageIO::New();
  io->DebugOn();

  typename ReaderType::Pointer reader = ReaderType::New();
  std::cout << "reader->GetUseStreaming(): " << reader->GetUseStreaming() << std::endl;
  std::cout << "done checking streaming usage" << std::endl;

  reader->SetImageIO( io );

  reader->SetFileName( inputFileName );

  typedef itk::StreamingImageFilter< ImageType5D, ImageType5D > StreamingFilter;
  typename StreamingFilter::Pointer streamer = StreamingFilter::New();
  streamer->SetInput( reader->GetOutput() );
  streamer->SetNumberOfStreamDivisions( numberOfStreamDivisions );

  ImageType5D::Pointer image5D =  ImageType5D::New();
  image5D = streamer->GetOutput();

  typedef itk::ImageFileWriter< ImageType5D > WriterType;
  typename WriterType::Pointer writer;
  writer = WriterType::New();
  writer->SetInput( streamer->GetOutput() );

  if ( writeSCIFIO )
    {
    itk::SCIFIOImageIO::Pointer ioOut = itk::SCIFIOImageIO::New();
    ioOut->DebugOn();
    writer->SetImageIO( ioOut );
    }

  reader->UpdateOutputInformation();
  io->SetSeries(seriesStart);
  reader->Modified();

  if ( allSeries )
    {
    seriesEnd = io->GetSeriesCount();
    }

  bool insertSeries = seriesEnd > (seriesStart + 1);

  int seriesCount = seriesEnd - seriesStart;
  int sigFigs = 0;

  while (seriesCount >= 10)
    {
    seriesCount /= 10;
    sigFigs++;
    }



  while ( seriesStart < seriesEnd )
    {

    std::string fileOut = outputFileName;

    if (insertSeries)
      {
      // Adjust file names if converting multiple series
      std::stringstream ssout;

      int currentSigFigs = 0;
      int currentSeries = seriesStart;

      while (currentSeries >= 10)
        {
        currentSeries /= 10;
        currentSigFigs++;
        }

      for (int i=0; i<(sigFigs - currentSigFigs); i++)
        {
        ssout << 0;
        }

      ssout << seriesStart;
      fileOut.insert(fileOut.find_first_of('.'), ssout.str());
      }

    std::cout << "updating image5D..." << std::endl;
    image5D->Update();					//crashes on "dead-A.ome.tiff" if writer gets updated
    std::cout << "image5D updated" << std::endl;


//------if the following code block gets executed image5D->Update() crashes
    writer->SetFileName( fileOut );

    try
      {
      writer->Update();
      }
    catch (itk::ExceptionObject &e)
      {
      std::cerr << e << std::endl;
      return EXIT_FAILURE;
      }
//------

    seriesStart++;

    if ( seriesStart < seriesEnd)
      {
      io->SetSeries(seriesStart);
      reader->Modified();
      }
    }

  std::string notes;
  itk::ExposeMetaData<std::string>( reader->GetMetaDataDictionary(), "Recording #1 Notes", notes );
  std::cout << "Notes: " << notes << std::endl;

  return EXIT_SUCCESS;
}



/**
 * Main method
 */
int main( int argc, char * argv [] )
{
  const char * inputFileName = argv[1];
  const char * outputFileName = argv[2];

  RunTest(inputFileName, outputFileName, 0, 1, 4, true, true);
  
  return EXIT_SUCCESS;
}
